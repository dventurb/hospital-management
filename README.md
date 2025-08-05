<div align ="center">
    <!--LOGO-->
    <a href="github.com/dventurb/hospital-management">
    <img src="https://github.com/dventurb/hospital-management/blob/gui-integration/assets/icons/common/icon.png" alt="Logo" width="64" height="64">
</a>

<!--PROJECT NAME-->
<h1>Hospital Management</h1>

<!--DESCRIPTION-->
<p align="center">
    Desktop hospital management application with CRUD operations, developed in C using GTK 4. 
</p>

<!--BADGES-->
<p>
     <img src="https://img.shields.io/badge/platform-linux-lightgrey" />
     <img src="https://img.shields.io/badge/GTK-4.14-blue" />
</p>

<h3>
    <a href="#about-project">About Project</a>
    <span>|</span>
    <a href="#installation">Installation</a>
    <span>|</span>
    <a href="#dependencies">Dependencies</a>
    <span>|</span>
    <a href="#features">Features</a>
</h3>
</div>

![screenshot](https://i.imgur.com/mkcD9r5.png)


## About Project
**Hospital Management** is a desktop application for mananing clients, doctors and medical appointments. This project was developed in C with GTK 4, with a focus on improving GUI development with C. The project was created as part of the Algorithms and Programming curricular unit at IPVC – ESTG.

## Installation 

```bash 
git clone https://github.com/dventurb/hospital-management.git
cd hospital-management
chmod +x install.sh
./install.sh
./hospital-management
```

## Dependencies
All dependencies are automatically installed by the install script (install.sh).
- **GTK 4** (`libgtk-4-dev`) 
- **GTKChart** (included in `ui/lib/gtkchart/`)
- **libharu** (`libhpdf-dev`)
- **hpdftbl** (included in `lib/hpdftbl/`)
- **crypt** (`libxcrypt-dev`)
- **cJSON** (`libcjson-dev`)
- **Build tools**: `gcc`, `make`, `pkg-config`

## Features 
- Modern user interface with **GTK 4**.
- Manage **clients**, **doctors**, and **appointments**. 
- Generate **PDF reports** using `libharu` and `hpdftbl`.
- Visualize data with **Pie Chart** and **Bar Chart**.
- Password with **SHA-256** and salt generation for unique hashes.
- Support for **dark** and **light** themes.



