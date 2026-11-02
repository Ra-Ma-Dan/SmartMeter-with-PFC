idf_component_register(SRC "nvs.c"
                        PREV_REQIRES esp_err.h freertos/FreeRTOS.h freertos/event_groups.h freertos/task.h esp_mac.h esp_wifi.h esp_event.h esp_log.h esp_netif_net_stack.h esp_netif.h lwip/inet.h lwip/netdb.h lwip/sockets.h lwip/lwip_napt.h lwip/err.h lwip/sys.h nvs/nvs_flash.h
                        INCLUDE_DIRS ".")

                                                                                                                                #include <string.h>

                                                                                                                                            #include "lwip/inet.h"
                                                                                                                                            #include "lwip/netdb.h"
                                                                                                                                            #include "lwip/sockets.h"
                                                                                                                                            #include "lwip/lwip_napt.h"
                                                                                                                                            #include "lwip/err.h"
                                                                                                                                            #include "lwip/sys.h"
                                                                                                                                            #include "wifi.h"