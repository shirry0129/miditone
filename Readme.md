# miditone
musicgame for koreisai in nitnc

## require
[OpenSiv3D](https://siv3d.github.io/) ver 0.4.0 or newer

[Boost](https://www.boost.org/) 1.71.0 or newer(asio/filesystem/Property Tree)

### for BalanceBoard
[wiiuse](https://github.com/wiiuse/wiiuse)

### macOS
newest Xcode 

### Windows
Microsoft Visual C++ 2017 15.7 or newer

Windows 10 SDK (10.0.17134.0) or newer

(need to use compatible with OpenSiv3D ver)

If it does't work, try downgrade Windows 10 SDK version.

## how to build
### button ver
add src/ to OpenSiv3D blank project, and build.

### BalanceBoard ver
1. add `MIDITONE_WIIBALANCEBOARD` to preprocesser macro.
2. add src/ to OpenSiv3D blank project, and build.

# Relating Repository
miditone-server <https://github.com/Yamamoto0773/miditone-server>

miditone-client <https://github.com/Yamamoto0773/miditone-client>

(miditone-client is already imported to this repository)

# LICENSE
GPL-v3.0
