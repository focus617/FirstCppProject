    find_program(OPENSSL_COMMAND
        NAMES openssl
        PATHS ${OPENSSL_INCLUDE_DIR}/../bin
        REQUIRED
    )

    set(CERT_OUPUT_DIR ${CMAKE_BINARY_DIR}/bin)

    execute_process(
        COMMAND ${OPENSSL_COMMAND} genrsa 2048
        OUTPUT_FILE key.pem
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(
        COMMAND ${OPENSSL_COMMAND} req -new -batch -config 
        ${CMAKE_CURRENT_LIST_DIR}/test.conf -key key.pem
        COMMAND ${OPENSSL_COMMAND} x509 -days 3650 -req -signkey key.pem
        OUTPUT_FILE cert.pem
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(
        COMMAND ${OPENSSL_COMMAND} req -x509 -new -config 
        ${CMAKE_CURRENT_LIST_DIR}/test.conf -key key.pem -sha256 -days 3650 -nodes -out cert2.pem -extensions SAN
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(
        COMMAND ${OPENSSL_COMMAND} genrsa 2048
        OUTPUT_FILE rootCA.key.pem
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(
        COMMAND ${OPENSSL_COMMAND} req -x509 -new -batch -config 
        ${CMAKE_CURRENT_LIST_DIR}/test.rootCA.conf -key rootCA.key.pem -days 1024
        OUTPUT_FILE rootCA.cert.pem
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(
        COMMAND ${OPENSSL_COMMAND} genrsa 2048
        OUTPUT_FILE client.key.pem
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(
        COMMAND ${OPENSSL_COMMAND} req -new -batch -config 
        ${CMAKE_CURRENT_LIST_DIR}/test.conf -key client.key.pem
        COMMAND ${OPENSSL_COMMAND} x509 -days 370 -req -CA rootCA.cert.pem -CAkey rootCA.key.pem -CAcreateserial
        OUTPUT_FILE client.cert.pem
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(
        COMMAND ${OPENSSL_COMMAND} genrsa -passout pass:test123! 2048
        OUTPUT_FILE key_encrypted.pem
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(
        COMMAND ${OPENSSL_COMMAND} req -new -batch -config ${CMAKE_CURRENT_LIST_DIR}/test.conf -key key_encrypted.pem
        COMMAND ${OPENSSL_COMMAND} x509 -days 3650 -req -signkey key_encrypted.pem
        OUTPUT_FILE cert_encrypted.pem
        WORKING_DIRECTORY ${CERT_OUPUT_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )