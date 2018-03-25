The simple mqtt protocol implement.
===================================

## Feature 

  - mqtt specification v3.1.1
  - -std=c99.
  - easy port.
  - easy using.

## Project Structure

                core
              /     \
             \/     \/
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
    fixed header  ---tcp---  fixed header
                      

## Demo 

  - ipc:build mqtt protocol in linux ipc.

