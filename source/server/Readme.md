
## For device verification
## net pipe
Client                              Server
1 pub key                        N pub keys
1 priv key                       1 priv key
---------                       ------------                        
|       |  sync with uuid ==>   |
|       |                       |
|       |  <== Challenge        |
|       |                       |
|       |  Answer ==>           |
|       |                       | map rinfo to uuid (update)


## Protocol < 1k per pack
|32 bytes| | 8 bytes | | 4 bytes      | | 0-128 bytes         | | 0-512 bytes | 
| token  | | msg id  | | truth length | | truth of encryption | |  payload    |


## sync 10 times per second
