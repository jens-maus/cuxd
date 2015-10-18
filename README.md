# CUx-Daemon
This repository hosts the CCU-Addon build environment for the CUx-Daemon (CUxD) project (http://cuxd.de) which provides a universal interface between the HomeMatic/CCU layer (ReGa HSS) and third-party external and virtual devices.

## Supported CCU Devices
An installation of the CUxD Addon is currently supported on the following CCU devices:
* HomeMatic CCU1
* HomeMatic CCU2
* RaspberryMatic (beta)

## Installation
1. Download the latest CUxD release.
2. Upload it via the HomeMatic WebUI provided Addon functionality.
3. Configure CUxD via http://homematic-ccu2/addons/cuxd

## Documentation
Documentation in form of pdf files can be downloaded from the 'docs' subdirectory of this repository.

## License
The CUxD build environment as published in this Github repository is provided under the BSD license. While source code files provided with the build environment are also released under the same license, binaries files (e.g. 'cuxd', 'redir.ccc', 'index.ccc') are licensed under a properitary closed-source license and just added to this repository to allow to automatically generate release archives.

## Authors
* CUxD is developed by Uwe Langhammer (uwe111) with certain parts being previously developed by Alex Krypthul.
* Parts of the build environment for CUxD has been developed by Jens Maus.
