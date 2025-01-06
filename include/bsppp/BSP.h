#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

#include <sourcepp/parser/Binary.h>

#include "LumpData.h"
#include "PakLump.h"

namespace bsppp {

constexpr auto BSP_SIGNATURE = sourcepp::parser::binary::makeFourCC("VBSP");
constexpr auto LZMA_VALVE_SIGNATURE = sourcepp::parser::binary::makeFourCC("LZMA");

enum class BSPLump : int32_t {
	UNKNOWN = -1,
	ENTITIES = 0,
	PLANES,
	TEXDATA,
	VERTEXES,
	VISIBILITY,
	NODES,
	TEXINFO,
	FACES,
	LIGHTING,
	OCCLUSION,
	LEAFS,
	FACEIDS,
	EDGES,
	SURFEDGES,
	MODELS,
	WORLDLIGHTS,
	LEAFFACES,
	LEAFBRUSHES,
	BRUSHES,
	BRUSHSIDES,
	AREAS,
	AREAPORTALS,
	S2004_PORTALS,
	UNUSED0 = S2004_PORTALS,
	SL4D2_PROPCOLLISION = S2004_PORTALS,
	S2004_CLUSTERS,
	UNUSED1 = S2004_CLUSTERS,
	SL4D2_PROPHULLS = S2004_CLUSTERS,
	S2004_PORTALVERTS,
	UNUSED2 = S2004_PORTALVERTS,
	SL4D2_PROPHULLVERTS = S2004_PORTALVERTS,
	S2004_CLUSTERPORTALS,
	UNUSED3 = S2004_CLUSTERPORTALS,
	SL4D2_PROPTRIS = S2004_CLUSTERPORTALS,
	DISPINFO,
	ORIGINALFACES,
	PHYSDISP,
	PHYSCOLLIDE,
	VERTNORMALS,
	VERTNORMALINDICES,
	S2004_DISP_LIGHTMAP_ALPHAS,
	UNUSED4 = S2004_DISP_LIGHTMAP_ALPHAS,
	DISP_VERTS,
	DISP_LIGHTMAP_SAMPLE_POSITIONS,
	GAME_LUMP,
	LEAFWATERDATA,
	PRIMITIVES,
	PRIMVERTS,
	PRIMINDICES,
	PAKFILE,
	CLIPPORTALVERTS,
	CUBEMAPS,
	TEXDATA_STRING_DATA,
	TEXDATA_STRING_TABLE,
	OVERLAYS,
	LEAFMINDISTTOWATER,
	FACE_MACRO_TEXTURE_INFO,
	DISP_TRIS,
	S2004_PHYSCOLLIDESURFACE,
	UNUSED5 = S2004_PHYSCOLLIDESURFACE,
	SL4D2_PROP_BLOB = S2004_PHYSCOLLIDESURFACE,
	WATEROVERLAYS,
	S2006_XBOX_LIGHTMAPPAGES,
	LEAF_AMBIENT_INDEX_HDR = S2006_XBOX_LIGHTMAPPAGES,
	S2006_XBOX_LIGHTMAPPAGEINFOS,
	LEAF_AMBIENT_INDEX = S2006_XBOX_LIGHTMAPPAGEINFOS,
	LIGHTING_HDR,
	WORLDLIGHTS_HDR,
	LEAF_AMBIENT_LIGHTING_HDR,
	LEAF_AMBIENT_LIGHTING,
	XBOX_XZIPPAKFILE,
	FACES_HDR,
	MAP_FLAGS,
	OVERLAY_FADES,
	L4D_OVERLAY_SYSTEM_LEVELS,
	UNUSED6 = L4D_OVERLAY_SYSTEM_LEVELS,
	L4D2_PHYSLEVEL,
	UNUSED7 = L4D2_PHYSLEVEL,
	ASW_DISP_MULTIBLEND,
	UNUSED8 = ASW_DISP_MULTIBLEND,

	COUNT,
};
constexpr int32_t BSP_LUMP_COUNT = 64;
static_assert(static_cast<std::underlying_type_t<BSPLump>>(BSPLump::COUNT) == BSP_LUMP_COUNT, "Incorrect lump count!");

class BSP {
	struct Lump {
		/// Byte offset into file
		uint32_t offset;
		/// Length of lump data
		uint32_t length;
		/// Lump format version
		uint32_t version;
		/// Uncompressed length if lump is compressed, else 0
		uint32_t uncompressedLength;
	};

	struct Header {
		/// Version of the BSP file
		uint32_t version;
		/// Lump metadata
		std::array<Lump, BSP_LUMP_COUNT> lumps;
		/// Map version number
		uint32_t mapRevision;
	};

public:
	explicit BSP(std::string path_, bool loadPatchFiles = true);

	explicit operator bool() const;

	static BSP create(std::string path, uint32_t version = 21, uint32_t mapRevision = 0);

	[[nodiscard]] uint32_t getVersion() const;

	void setVersion(uint32_t version);

	[[nodiscard]] uint32_t getMapRevision() const;

	void setMapRevision(uint32_t mapRevision);

	[[nodiscard]] bool hasLump(BSPLump lumpIndex) const;

	[[nodiscard]] bool isLumpCompressed(BSPLump lumpIndex) const;

	[[nodiscard]] uint32_t getLumpVersion(BSPLump lumpIndex) const;

	[[nodiscard]] std::optional<std::vector<std::byte>> getLumpData(BSPLump lumpIndex, bool noDecompression = false) const;

	template<BSPLump Lump>
	[[nodiscard]] auto getLumpData() const {
		if constexpr (Lump == BSPLump::PLANES) {
			return this->parsePlanes();
		} else if constexpr (Lump == BSPLump::TEXDATA) {
			return this->parseTextureData();
		} else if constexpr (Lump == BSPLump::VERTEXES) {
			return this->parseVertices();
		} else if constexpr (Lump == BSPLump::NODES) {
			return this->parseNodes();
		} else if constexpr (Lump == BSPLump::TEXINFO) {
			return this->parseTextureInfo();
		} else if constexpr (Lump == BSPLump::FACES) {
			return this->parseFaces();
		} else if constexpr (Lump == BSPLump::EDGES) {
			return this->parseEdges();
		} else if constexpr (Lump == BSPLump::SURFEDGES) {
			return this->parseSurfEdges();
		} else if constexpr (Lump == BSPLump::MODELS) {
			return this->parseBrushModels();
		} else if constexpr (Lump == BSPLump::ORIGINALFACES) {
			return this->parseOriginalFaces();
		} else if constexpr (Lump == BSPLump::GAME_LUMP) {
			return this->parseGameLumps(true);
		} else {
			return this->getLumpData(Lump);
		}
	}

	/// BSP::setGameLump should be used for writing game lumps as they need special handling. Paklump can be
	/// written here but compression is unsupported, prefer using bsppp::PakLump or your favorite zip library
	/// instead. Valid compressLevel range is 0 to 9, 0 is considered off, 9 the slowest and most compressiest
	bool setLump(BSPLump lumpIndex, uint32_t version, std::span<const std::byte> data, uint8_t compressLevel = 0);

	[[nodiscard]] bool isGameLumpCompressed(BSPGameLump::Signature signature) const;

	[[nodiscard]] uint16_t getGameLumpVersion(BSPGameLump::Signature signature);

	[[nodiscard]] std::optional<std::vector<std::byte>> getGameLumpData(BSPGameLump::Signature signature) const;

	bool setGameLump(BSPGameLump::Signature signature, uint16_t version, std::span<const std::byte> data, uint8_t compressLevel = 0);

	/// Reset changes made to a lump before they're written to disk
	void resetLump(BSPLump lumpIndex);

	/// Resets ALL in-memory modifications (version, all lumps including game lumps, map revision)
	void reset();

	void createLumpPatchFile(BSPLump lumpIndex) const;

	bool setLumpFromPatchFile(const std::string& lumpFilePath);

	bool bake(std::string_view outputPath = "");

protected:
	bool readHeader();

	[[nodiscard]] std::vector<BSPPlane> parsePlanes() const;

	[[nodiscard]] std::vector<BSPTextureData> parseTextureData() const;

	[[nodiscard]] std::vector<BSPVertex> parseVertices() const;

	[[nodiscard]] std::vector<BSPNode> parseNodes() const;

	[[nodiscard]] std::vector<BSPTextureInfo> parseTextureInfo() const;

	[[nodiscard]] std::vector<BSPFace> parseFaces() const;

	[[nodiscard]] std::vector<BSPEdge> parseEdges() const;

	[[nodiscard]] std::vector<BSPSurfEdge> parseSurfEdges() const;

	[[nodiscard]] std::vector<BSPBrushModel> parseBrushModels() const;

	[[nodiscard]] std::vector<BSPFace> parseOriginalFaces() const;

	[[nodiscard]] std::vector<BSPGameLump> parseGameLumps(bool decompress) const;

	[[nodiscard]] static std::optional<std::vector<std::byte>>	compressLumpData(const std::span<const std::byte> data, uint8_t compressLevel = 6);

	[[nodiscard]] static std::optional<std::vector<std::byte>>	decompressLumpData(const std::span<const std::byte> data);

	std::string path;
	Header header{};

	uint32_t stagedVersion{};
	std::unordered_map<uint32_t, std::pair<Lump, std::vector<std::byte>>> stagedLumps;
	std::vector<BSPGameLump> stagedGameLumps;
	uint32_t stagedMapRevision{};

	// Slightly different header despite using the same version just to be quirky
	bool isL4D2 = false;
};

} // namespace bsppp
