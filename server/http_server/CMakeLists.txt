idf_component_register(SRCS "http_server.c"
                       INCLUDE_DIRS "."
                       REQUIRES esp_http_server
                       EMBED_TXTFILES "../webpage/index.html"
                                      "../webpage/style.css"
                                      "../webpage/scripts.js")
