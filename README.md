The simple mqtt protocol implementation
=======================================

[![Build Status](https://travis-ci.org/Shylock-Hg/mqttor.svg?branch=master)](https://travis-ci.org/Shylock-Hg/mqttor)
[![codecov](https://codecov.io/gh/Shylock-Hg/mqttor/branch/master/graph/badge.svg)](https://codecov.io/gh/Shylock-Hg/mqttor)

## Feature

- mqtt specification v3.1.1
- -std=c99.
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

- ipc:build mqtt protocol in linux ipc.

