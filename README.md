The simple mqtt protocol implementation
=======================================

[![Build Status](https://travis-ci.org/Shylock-Hg/mqttor.svg?branch=master)](https://travis-ci.org/Shylock-Hg/mqttor)
[![codecov](https://codecov.io/gh/Shylock-Hg/mqttor/branch/master/graph/badge.svg)](https://codecov.io/gh/Shylock-Hg/mqttor)
[![CodeFactor](https://www.codefactor.io/repository/github/shylock-hg/mqttor/badge)](https://www.codefactor.io/repository/github/shylock-hg/mqttor)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Shylock-Hg/mqttor.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Shylock-Hg/mqttor/context:cpp)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)

## Feature

- mqtt specification v3.1.1
- easy port.
- easy using.

## Project Structure

                core
                  |
                  |
               session
               |     |
               |     |
            broker client
      ------------------------
           python wrapper

## MQTT communication detail

      client                    broker
         |                        |
      payload                  payload
         |                        |
  variable header          variable header
         |                        |
    fixed header  \-\-\-tcp\-\-\- fixed header

## Demo

### Client

Install by `make -j8 && sudo make install`, check usage by \
`mqttor_client_at -h`, `mqttor_client -h`.

