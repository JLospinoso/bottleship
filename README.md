Bottleship
==

Bottleship is a Battleship-style game engine written in Modern C++. It is under active development.

For information about the protocol, see [The Bottleship Protocol](https://github.com/jaarky/bottleship/wiki/The-Bottleship-Protocol).

Experimenting with Bottleship
==

There are two components to Bottleship: the game server and the client. You may setup a game server with the published
Docker container, e.g.:

```sh
docker run -p 8000:8000 jaarky/bottleship:latest ./bottleship
```

or

```sh
docker run -p 8000:8000 quay.io/jaarky/bottleship:latest ./bottleship
```

By default, `bottleship` listens on `:8000`. You may interact with the gameserver with the [web client](http://htmlpreview.github.io/?https://raw.githubusercontent.com/jaarky/bottleship/master/web/webclient.html).

Getting Involved
==

If you would like to get involved, join us on Gitter! [![Join the chat at https://gitter.im/boship/Lobby](https://badges.gitter.im/boship/Lobby.svg)](https://gitter.im/boship/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Containers
==
* Docker Hub - `docker pull jaarky/bottleship` [![Docker Automated buil](https://img.shields.io/docker/automated/jaarky/bottleship.svg)](https://hub.docker.com/r/jaarky/bottleship/)
* quay.io - `docker pull quay.io/jaarky/bottleship` [![Docker Repository on Quay](https://quay.io/repository/jaarky/bottleship/status "Docker Repository on Quay")](https://quay.io/repository/jaarky/bottleship)

CircleCI
==

* `master`: [![CircleCI](https://circleci.com/gh/jaarky/bottleship/tree/master.svg?style=svg)](https://circleci.com/gh/jaarky/bottleship/tree/master)
* `dev`: [![CircleCI](https://circleci.com/gh/jaarky/bottleship/tree/dev.svg?style=svg)](https://circleci.com/gh/jaarky/bottleship/tree/dev)

Codeship
==

* `master`: [ ![Codeship Status for jaarky/bottleship](https://app.codeship.com/projects/e831eb40-b10a-0134-101a-1a4c0cdb2aa0/status?branch=master)](https://app.codeship.com/projects/193212)
* `dev`: [ ![Codeship Status for jaarky/bottleship](https://app.codeship.com/projects/e831eb40-b10a-0134-101a-1a4c0cdb2aa0/status?branch=dev)](https://app.codeship.com/projects/193212)
