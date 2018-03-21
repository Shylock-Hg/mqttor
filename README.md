The simple mqtt protocol implement.
===================================

## Feature 

  - std c language.
  - easy port.

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

