<div>
  <img align="left" width="98px" src="https://github.com/craftablescience/sourcepp/blob/main/branding/logo.png?raw=true" alt="The Source Pretty Parsers logo. A printer-esque device is scanning a page with hex codes and printing a picture of Cordon Freeman." />
  <h1>Source Pretty Parsers</h1>
</div>

![Build Status](https://img.shields.io/github/actions/workflow/status/craftablescience/sourcepp/build.yml?logo=github&label=Build)
![License](https://img.shields.io/github/license/craftablescience/sourcepp?label=License)
![Discord](https://img.shields.io/discord/678074864346857482?label=Discord&logo=Discord&logoColor=%23FFFFFF)

Several modern C++20 libraries for sanely parsing Valve formats, rolled into one big repository.

### Included Libraries

<table>
  <tr>
    <th>Name</th>
    <th>Supports</th>
    <th>Create</th>
    <th>Read</th>
    <th>Write</th>
    <th>Wrappers</th>
    <th>Special Thanks</th>
  </tr>
  <tr>
    <td>dmxpp</td>
    <td>
      <ul>
        <li><a href="https://developer.valvesoftware.com/wiki/DMX">DMX</a> Binary v1-5</li>
      </ul>
    </td>
    <td align="center">❌</td>
    <td align="center">✅</td>
    <td align="center">❌</td>
    <td align="center"></td>
    <td></td>
  </tr>
  <tr>
    <td>fgdpp</td>
    <td>
      <ul>
        <li><a href="https://developer.valvesoftware.com/wiki/FGD">FGD</a></li>
        <li><a href="https://github.com/TeamSpen210/HammerAddons/wiki/Unified-FGD">TeamSpen's Unified FGD</a></li>
      </ul>
    </td>
    <td align="center">❌</td>
    <td align="center">✅</td>
    <td align="center">❌</td>
    <td align="center"></td>
    <td>FGD parser contributed by <a href="https://github.com/Trico-Everfire">@Trice Everfire</a></td>
  </tr>
  <tr>
    <td>kvpp</td>
    <td>
      <ul>
        <li><a href="https://developer.valvesoftware.com/wiki/KeyValues">KeyValues</a> v1<sup>*</sup></li>
      </ul>
    </td>
    <td align="center">❌</td>
    <td align="center">✅</td>
    <td align="center">❌</td>
    <td align="center"></td>
    <td></td>
  </tr>
  <tr>
    <td>mdlpp</td>
    <td>
      <ul>
        <li><a href="https://developer.valvesoftware.com/wiki/MDL_(Source)">MDL</a> v44-49<sup>&dagger;</sup></li>
        <li><a href="https://developer.valvesoftware.com/wiki/VTX">VTX</a> v7</li>
        <li><a href="https://developer.valvesoftware.com/wiki/VVD">VVD</a> v4</li>
      </ul>
    </td>
    <td align="center">❌</td>
    <td align="center">✅</td>
    <td align="center">❌</td>
    <td align="center"></td>
    <td></td>
  </tr>
  <tr>
    <td>steampp</td>
    <td>
      <ul>
        <li>Finding Steam install folder</li>
        <li>Finding installed Steam games</li>
      </ul>
    </td>
    <td align="center">n/a</td>
    <td align="center">✅</td>
    <td align="center">n/a</td>
    <td align="center"></td>
    <td>Based on the <a href="https://github.com/Trico-Everfire/SteamAppPathProvider">SteamAppPathProvider</a> library by <a href="https://github.com/Trico-Everfire">@Trice Everfire</a> and <a href="https://momentum-mod.org">Momentum Mod</a> contributors.</td>
  </tr>
  <tr>
    <td>vicepp</td>
    <td>
      <ul>
        <li><a href="https://developer.valvesoftware.com/wiki/VICE">VICE</a> Encrypted Files</li>
      </ul>
    </td>
    <td align="center">✅</td>
    <td align="center">✅</td>
    <td align="center">✅</td>
    <td align="center">C<br>C#</td>
    <td></td>
  </tr>
  <tr>
    <td>vpkpp</td>
    <td>
      <ul>
        <li><a href="https://developer.valvesoftware.com/wiki/Bonus_Maps">BMZ</a></li>
        <li><a href="https://developer.valvesoftware.com/wiki/BSP_(Source)">BSP</a> v17-27</li>
        <li>FPX v10</li>
        <li><a href="https://developer.valvesoftware.com/wiki/GCF_archive">GCF</a> v6</li>
        <li>GMA v1-3</li>
        <li>GRP</li>
        <li><a href="https://quakewiki.org/wiki/.pak">PAK</a></li>
        <li><a href="https://docs.godotengine.org/en/stable/tutorials/export/exporting_pcks.html">PCK</a> v1-2</li>
        <li><a href="https://developer.valvesoftware.com/wiki/VPK">VPK</a> v1-2</li>
        <li>ZIP</li>
      </ul>
    </td>
    <td align="center">✅<sup>&Dagger;</sup></td>
    <td align="center">✅</td>
    <td align="center">✅</td>
    <td align="center">C<br>C#</td>
    <td>GCF parser contributed by <a href="https://github.com/caatge">@bt</a> and <a href="https://github.com/ymgve">@ymgve</a></td>
  </tr>
  <tr>
    <td>vtfpp</td>
    <td>
      <ul>
        <li><a href="https://developer.valvesoftware.com/wiki/VTF_(Valve_Texture_Format)">VTF</a> v7.0-7.6</li>
      </ul>
    </td>
    <td align="center">❌</td>
    <td align="center">✅</td>
    <td align="center">❌</td>
    <td align="center"></td>
    <td></td>
  </tr>
</table>

(\*) Many text-based formats in Source are close to (if not identical to) KeyValues v1, such as [VDF](https://developer.valvesoftware.com/wiki/VDF), [VMT](https://developer.valvesoftware.com/wiki/VMT), and [VMF](https://developer.valvesoftware.com/wiki/VMF_(Valve_Map_Format)).

(&dagger;) The MDL parser is not complete. It is usable in its current state, but it does not currently parse more complex components like animations. This parser is still in development.

(&Dagger;) Currently only VPK creation is supported.
