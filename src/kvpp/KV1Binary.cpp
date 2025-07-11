#include <kvpp/KV1Binary.h>

#include <format>
#include <functional>

#include <kvpp/KV1.h>
#include <sourcepp/FS.h>

using namespace kvpp;
using namespace sourcepp;

std::string_view KV1BinaryElement::getKey() const {
	return this->key;
}

void KV1BinaryElement::setKey(std::string_view key_) {
	this->key = key_;
}

const KV1BinaryValue& KV1BinaryElement::getValue() const {
	return this->value;
}

void KV1BinaryElement::setValue(KV1BinaryValue value_) {
	this->value = std::move(value_);
}

KV1BinaryElement& KV1BinaryElement::operator=(KV1BinaryValue value_) {
	this->setValue(std::move(value_));
	return *this;
}

bool KV1BinaryElement::hasChild(std::string_view childKey) const {
	return !this->operator[](childKey).isInvalid();
}

uint64_t KV1BinaryElement::getChildCount() const {
	return this->children.size();
}

uint64_t KV1BinaryElement::getChildCount(std::string_view childKey) const {
	uint64_t count = 0;
	for (const KV1BinaryElement& element : this->children) {
		if (string::iequals(element.key, childKey)) {
			++count;
		}
	}
	return count;
}

const std::vector<KV1BinaryElement>& KV1BinaryElement::getChildren() const {
	return this->children;
}

KV1BinaryElement& KV1BinaryElement::addChild(std::string_view key_) {
	KV1BinaryElement elem;
	elem.setKey(key_);
	this->children.push_back(elem);
	return this->children.back();
}

const KV1BinaryElement& KV1BinaryElement::operator[](unsigned int n) const {
	return this->children.at(n);
}

KV1BinaryElement& KV1BinaryElement::operator[](unsigned int n) {
	return this->children.at(n);
}

const KV1BinaryElement& KV1BinaryElement::operator[](std::string_view childKey) const {
	return this->operator()(childKey);
}

KV1BinaryElement& KV1BinaryElement::operator[](std::string_view childKey) {
	return this->operator()(childKey);
}

const KV1BinaryElement& KV1BinaryElement::operator()(std::string_view childKey) const {
	for (const auto& element : this->children) {
		if (string::iequals(element.getKey(), childKey)) {
			return element;
		}
	}
	return getInvalid();
}

KV1BinaryElement& KV1BinaryElement::operator()(std::string_view childKey) {
	for (auto& element : this->children) {
		if (string::iequals(element.getKey(), childKey)) {
			return element;
		}
	}
	return this->addChild(childKey);
}

const KV1BinaryElement& KV1BinaryElement::operator()(std::string_view childKey, unsigned int n) const {
	unsigned int count = 0;
	for (const auto& element : this->children) {
		if (string::iequals(element.getKey(), childKey)) {
			if (count == n) {
				return element;
			}
			++count;
		}
	}
	return getInvalid();
}

KV1BinaryElement& KV1BinaryElement::operator()(std::string_view childKey, unsigned int n) {
	unsigned int count = 0;
	for (auto& element: this->children) {
		if (string::iequals(element.getKey(), childKey)) {
			if (count == n) {
				return element;
			}
			++count;
		}
	}
	return this->addChild(childKey);
}

void KV1BinaryElement::removeChild(unsigned int n) {
	if (this->children.size() > n) {
		this->children.erase(this->children.begin() + n);
	}
}

void KV1BinaryElement::removeChild(std::string_view childKey, int n) {
	unsigned int count = 0;
	for (auto element = this->children.begin(); element != this->children.end(); ++element) {
		if (string::iequals(element->getKey(), childKey)) {
			if (n < 0 || count == n) {
				element = this->children.erase(element);
				if (count == n) {
					return;
				}
			}
			++count;
		}
	}
}

bool KV1BinaryElement::isInvalid() const {
	return this == &getInvalid();
}

const KV1BinaryElement& KV1BinaryElement::getInvalid() {
	static KV1BinaryElement element;
	return element;
}

void KV1BinaryElement::read(BufferStreamReadOnly& stream, std::vector<KV1BinaryElement>& elements, const parser::text::EscapeSequenceMap& escapeSequences) {
	for (;;) {
		const auto type = stream.read<KV1BinaryValueType>();
		if (type == KV1BinaryValueType::COUNT) {
			break;
		}
		KV1BinaryElement& element = elements.emplace_back();
		element.key = stream.read_string();
		switch (type) {
			using enum KV1BinaryValueType;
			case CHILDREN:
				KV1BinaryElement::read(stream, element.children, escapeSequences);
				break;
			case STRING:
				element.value = stream.read_string();
				break;
			case INT32:
				element.value = stream.read<int32_t>();
				break;
			case FLOAT:
				element.value = stream.read<float>();
				break;
			case POINTER:
				element.value = reinterpret_cast<void*>(stream.read<int32_t>());
				break;
			case WSTRING: {
				const auto len = stream.read<uint16_t>();
				std::wstring value;
				for (int i = 0; i < len; i++) {
					value += stream.read<char16_t>();
				}
				element.value = value;
				break;
			}
			case COLOR_RGBA: {
				math::Vec4ui8 value;
				value[0] = stream.read<uint8_t>();
				value[1] = stream.read<uint8_t>();
				value[2] = stream.read<uint8_t>();
				value[3] = stream.read<uint8_t>();
				element.value = value;
				break;
			}
			case UINT64:
				element.value = stream.read<uint64_t>();
				break;
			case COUNT:
				break;
		}
	}
}

void KV1BinaryElement::write(BufferStream& stream, const std::vector<KV1BinaryElement>& elements, const parser::text::EscapeSequenceMap& escapeSequences) {
	for (const auto& element : elements) {
		const auto type = static_cast<KV1BinaryValueType>(element.getValue().index());
		stream
			.write(type)
			.write(element.getKey());
		switch (type) {
			using enum KV1BinaryValueType;
			case CHILDREN:
				KV1BinaryElement::write(stream, element.children, escapeSequences);
				break;
			case STRING:
				stream.write(*element.getValue<std::string>());
				break;
			case INT32:
				stream.write(*element.getValue<int32_t>());
				break;
			case FLOAT:
				stream.write(*element.getValue<float>());
				break;
			case POINTER:
				stream.write(static_cast<int32_t>(reinterpret_cast<uint64_t>(*element.getValue<void*>())));
				break;
			case WSTRING: {
				const auto val = *element.getValue<std::wstring>();
				stream
					.write<uint16_t>(val.size() + 1)
					.write(reinterpret_cast<const char16_t*>(val.data()), val.size() * sizeof(char16_t) / sizeof(wchar_t))
					.write<uint16_t>(0);
				break;
			}
			case COLOR_RGBA: {
				const auto val = *element.getValue<math::Vec4ui8>();
				stream << val[0] << val[1] << val[2] << val[3];
				break;
			}
			case UINT64:
				stream.write(*element.getValue<uint64_t>());
				break;
			case COUNT:
				break;
		}
	}
	stream.write(KV1BinaryValueType::COUNT);
}

KV1Binary::KV1Binary(std::span<const std::byte> kv1Data, bool useEscapeSequences_)
		: KV1BinaryElement()
		, useEscapeSequences(useEscapeSequences_) {
	if (kv1Data.empty()) {
		return;
	}
	BufferStreamReadOnly stream{kv1Data.data(), kv1Data.size()};
	KV1BinaryElement::read(stream, this->children, parser::text::getDefaultEscapeSequencesOrNone(this->useEscapeSequences));
}

std::vector<std::byte> KV1Binary::bake() const {
	std::vector<std::byte> buffer;
	BufferStream stream{buffer};
	KV1BinaryElement::write(stream, this->children, parser::text::getDefaultEscapeSequencesOrNone(this->useEscapeSequences));
	buffer.resize(stream.size());
	return buffer;
}

void KV1Binary::bake(const std::string& kv1Path) const  {
	fs::writeFileBuffer(kv1Path, this->bake());
}

std::string KV1Binary::bakeText() const {
	KV1Writer writer;
	std::function<void(const KV1BinaryElement&, KV1ElementWritable<>&)> recurseBinaryKeyValues;
	recurseBinaryKeyValues = [&recurseBinaryKeyValues](const KV1BinaryElement& element, KV1ElementWritable<>& kv) {
		auto& child = kv.addChild(element.getKey());
		switch (static_cast<KV1BinaryValueType>(element.getValue().index())) {
			using enum KV1BinaryValueType;
			case CHILDREN:
				for (const auto& elementChild : element.getChildren()) {
					recurseBinaryKeyValues(elementChild, child);
				}
				break;
			case STRING:
				child = *element.getValue<std::string>();
				break;
			case INT32:
				child = *element.getValue<int32_t>();
				break;
			case FLOAT:
				child = *element.getValue<float>();
				break;
			case POINTER:
				child = reinterpret_cast<int64_t>(*element.getValue<void*>());
				break;
			case WSTRING:
				break;
			case COLOR_RGBA: {
				const auto val = *element.getValue<math::Vec4ui8>();
				child = std::format("[{} {} {} {}]", val[0], val[1], val[2], val[3]);
				break;
			}
			case UINT64:
				child = std::format("{}", *element.getValue<uint64_t>());
				break;
			case COUNT:
				break;
		}
	};
	recurseBinaryKeyValues(*this, writer);
	return writer.bake();
}

void KV1Binary::bakeText(const std::string& kv1Path) const {
	fs::writeFileText(kv1Path, this->bakeText());
}
