<a id="top"></a>

<div align="center">
    <h3>CMusic</h3>
</div>

---

### Table of Contents

- [Description](#description)
  - [Built With](#built-with)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [Basic usage](#basic-usage)
  - [Program behavior](#program-behavior)
- [Contributing](#contributing)
- [Acknoledgements](#acknoledgements)
- [License](#license)

---

## Description

CMusic is a program that lets you manage your favorite music using the Spotify API.

The program is able to call the Spotify API to get informations about albums, artists, playlists and tracks directly from the terminal!

You will be able to add/remove tracks from your playlists, update your playlists informations, follow/unfollow artists and playlists and discover your favorite tracks/artists.

### Built With

[![C](https://skillicons.dev/icons?i=c&theme=light)](https://www.c-language.org/)
[![CMake](https://skillicons.dev/icons?i=cmake&theme=light)](https://cmake.org/)

#### AND

[![cURL](https://img.shields.io/badge/-curl-073551?style=for-the-badge&logo=curl&logoColor=white)](https://curl.se/libcurl/)
[![Spotify API](https://img.shields.io/badge/Spotify-1ED760?style=for-the-badge&logo=spotify&logoColor=white)](https://developer.spotify.com/)

## Getting Started

### Prerequisites

- CMake
- cURL
- Criterion (for testing only)
- A Spotify account

### Installation

1. Fork the [Project repository](https://github.com/NestorNebula/cmusic)
2. Clone the forked repository to your local machine
   ```
   git clone git@github.com:<your username>/<repo name>.git
   ```
3. Update remote URL

   ```
   # SSH:
   git remote add upstream git@github.com:NestorNebula/cmusic.git

   # HTTPS:
   git remote add upstream https://github.com/NestorNebula/cmusic.git
   ```
4. From the root directory, build the configuration files
   ```
   cmake -B build/ 
   ```
5. Go to the "build" directory created by the previous command and build the program
   ```
   make
   ```
6. Get a valid Spotify Web API access token (Read the [Get a valid access token](https://github.com/NestorNebula/cmusic/blob/main/API.md#get-a-valid-access-token) part of the project's [API docs](https://github.com/NestorNebula/cmusic/blob/main/API.md)).
7. Run the program using your access token
   ```
   ./cmusic token
   ```

#### Tests

Tests suites can be found in the "tests" directory.

New files in this directory with the extension .c will automatically be considered as tests suites.

To run tests, follow these steps:

1. From the root directory of the project, go to the "tests" directory
   ```
   cd tests
   ```
2. "Build" the tests configuration files
   ```
   cmake -B build/ 
   ```
3. Go to the "build" directory created by the previous command and build the test file
   ```
   make
   ```
4. Run the tests
   ```
   ./cmusic-tests
   ```

## Usage

### Basic usage

Once the app is started, its usage should be pretty straightforward. 

The user is offered multiple options like searching in the catalog, managing their playlists and their followed artists/playlists, reading informations about their favorites artists/tracks.

### Program behavior

The user should keep in mind that any change made using this application can't be revoked and will impact their Spotify account.

Some errors can occur during API calls to the Spotify API. In this case, to avoid invalid data to be displayed or operations to be done incorrectly, the program will terminate. This is expected behavior and means that the problem that occurred isn't due to the program.

To learn more about errors that can occur during API calls, read the project's <a href="https://github.com/NestorNebula/cmusic/blob/main/API.md">API docs</a>.

## Contributing

If you find any issue while using the program or want to contribute, please <a href="https://github.com/NestorNebula/cmusic/issues">open an issue</a>.

Only open issues if you previously made sure that the issue is due to the program and not to the API.

## Acknoledgements

- [cJSON](https://github.com/DaveGamble/cJSON)
- [Criterion](https://github.com/Snaipe/Criterion)
- [FFF (Fake Function Framework)](https://github.com/meekrosoft/fff)

## License

[![MIT License](https://img.shields.io/badge/License-MIT-darkcyan.svg?style=for-the-badge)](https://github.com/NestorNebula/cmusic/blob/main/LICENSE)

© Noa Houssier - 2025

<p align='right'>(<a href='#top'>go back to the top</a>)</p>
