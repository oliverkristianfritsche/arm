idf_component_register(SRCS "main.c"
                            "controllers/motors/mg90s.c"
                            "controllers/wifi/wifi_manager.c"
                            "controllers/statemachine/statemachine.c"
                            "controllers/tcp/TCP.c"
                      INCLUDE_DIRS "."
                                   "controllers/motors"
                                   "controllers/wifi"
                                   "controllers/statemachine"
                                   "controllers/tcp")
