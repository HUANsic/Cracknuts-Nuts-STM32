################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../library/aes.c \
../library/aesni.c \
../library/aria.c \
../library/asn1parse.c \
../library/asn1write.c \
../library/base64.c \
../library/camellia.c \
../library/ccm.c \
../library/chacha20.c \
../library/chachapoly.c \
../library/cipher.c \
../library/cipher_wrap.c \
../library/cmac.c \
../library/ctr_drbg.c \
../library/debug.c \
../library/des.c \
../library/dhm.c \
../library/ecdh.c \
../library/ecdsa.c \
../library/ecjpake.c \
../library/ecp.c \
../library/ecp_curves.c \
../library/gcm.c \
../library/hkdf.c \
../library/hmac_drbg.c \
../library/md.c \
../library/md5.c \
../library/memory_buffer_alloc.c \
../library/mps_reader.c \
../library/mps_trace.c \
../library/nist_kw.c \
../library/oid.c \
../library/padlock.c \
../library/pem.c \
../library/pk.c \
../library/pk_wrap.c \
../library/pkparse.c \
../library/pkwrite.c \
../library/platform.c \
../library/platform_util.c \
../library/ripemd160.c \
../library/sha1.c \
../library/sha256.c \
../library/sha512.c 

OBJS += \
./library/aes.o \
./library/aesni.o \
./library/aria.o \
./library/asn1parse.o \
./library/asn1write.o \
./library/base64.o \
./library/camellia.o \
./library/ccm.o \
./library/chacha20.o \
./library/chachapoly.o \
./library/cipher.o \
./library/cipher_wrap.o \
./library/cmac.o \
./library/ctr_drbg.o \
./library/debug.o \
./library/des.o \
./library/dhm.o \
./library/ecdh.o \
./library/ecdsa.o \
./library/ecjpake.o \
./library/ecp.o \
./library/ecp_curves.o \
./library/gcm.o \
./library/hkdf.o \
./library/hmac_drbg.o \
./library/md.o \
./library/md5.o \
./library/memory_buffer_alloc.o \
./library/mps_reader.o \
./library/mps_trace.o \
./library/nist_kw.o \
./library/oid.o \
./library/padlock.o \
./library/pem.o \
./library/pk.o \
./library/pk_wrap.o \
./library/pkparse.o \
./library/pkwrite.o \
./library/platform.o \
./library/platform_util.o \
./library/ripemd160.o \
./library/sha1.o \
./library/sha256.o \
./library/sha512.o 

C_DEPS += \
./library/aes.d \
./library/aesni.d \
./library/aria.d \
./library/asn1parse.d \
./library/asn1write.d \
./library/base64.d \
./library/camellia.d \
./library/ccm.d \
./library/chacha20.d \
./library/chachapoly.d \
./library/cipher.d \
./library/cipher_wrap.d \
./library/cmac.d \
./library/ctr_drbg.d \
./library/debug.d \
./library/des.d \
./library/dhm.d \
./library/ecdh.d \
./library/ecdsa.d \
./library/ecjpake.d \
./library/ecp.d \
./library/ecp_curves.d \
./library/gcm.d \
./library/hkdf.d \
./library/hmac_drbg.d \
./library/md.d \
./library/md5.d \
./library/memory_buffer_alloc.d \
./library/mps_reader.d \
./library/mps_trace.d \
./library/nist_kw.d \
./library/oid.d \
./library/padlock.d \
./library/pem.d \
./library/pk.d \
./library/pk_wrap.d \
./library/pkparse.d \
./library/pkwrite.d \
./library/platform.d \
./library/platform_util.d \
./library/ripemd160.d \
./library/sha1.d \
./library/sha256.d \
./library/sha512.d 


# Each subdirectory must supply rules for building sources it contributes
library/%.o library/%.su library/%.cyclo: ../library/%.c library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F439xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-library

clean-library:
	-$(RM) ./library/aes.cyclo ./library/aes.d ./library/aes.o ./library/aes.su ./library/aesni.cyclo ./library/aesni.d ./library/aesni.o ./library/aesni.su ./library/aria.cyclo ./library/aria.d ./library/aria.o ./library/aria.su ./library/asn1parse.cyclo ./library/asn1parse.d ./library/asn1parse.o ./library/asn1parse.su ./library/asn1write.cyclo ./library/asn1write.d ./library/asn1write.o ./library/asn1write.su ./library/base64.cyclo ./library/base64.d ./library/base64.o ./library/base64.su ./library/camellia.cyclo ./library/camellia.d ./library/camellia.o ./library/camellia.su ./library/ccm.cyclo ./library/ccm.d ./library/ccm.o ./library/ccm.su ./library/chacha20.cyclo ./library/chacha20.d ./library/chacha20.o ./library/chacha20.su ./library/chachapoly.cyclo ./library/chachapoly.d ./library/chachapoly.o ./library/chachapoly.su ./library/cipher.cyclo ./library/cipher.d ./library/cipher.o ./library/cipher.su ./library/cipher_wrap.cyclo ./library/cipher_wrap.d ./library/cipher_wrap.o ./library/cipher_wrap.su ./library/cmac.cyclo ./library/cmac.d ./library/cmac.o ./library/cmac.su ./library/ctr_drbg.cyclo ./library/ctr_drbg.d ./library/ctr_drbg.o ./library/ctr_drbg.su ./library/debug.cyclo ./library/debug.d ./library/debug.o ./library/debug.su ./library/des.cyclo ./library/des.d ./library/des.o ./library/des.su ./library/dhm.cyclo ./library/dhm.d ./library/dhm.o ./library/dhm.su ./library/ecdh.cyclo ./library/ecdh.d ./library/ecdh.o ./library/ecdh.su ./library/ecdsa.cyclo ./library/ecdsa.d ./library/ecdsa.o ./library/ecdsa.su ./library/ecjpake.cyclo ./library/ecjpake.d ./library/ecjpake.o ./library/ecjpake.su ./library/ecp.cyclo ./library/ecp.d ./library/ecp.o ./library/ecp.su ./library/ecp_curves.cyclo ./library/ecp_curves.d ./library/ecp_curves.o ./library/ecp_curves.su ./library/gcm.cyclo ./library/gcm.d ./library/gcm.o ./library/gcm.su ./library/hkdf.cyclo ./library/hkdf.d ./library/hkdf.o ./library/hkdf.su ./library/hmac_drbg.cyclo ./library/hmac_drbg.d ./library/hmac_drbg.o ./library/hmac_drbg.su ./library/md.cyclo ./library/md.d ./library/md.o ./library/md.su ./library/md5.cyclo ./library/md5.d ./library/md5.o ./library/md5.su ./library/memory_buffer_alloc.cyclo ./library/memory_buffer_alloc.d ./library/memory_buffer_alloc.o ./library/memory_buffer_alloc.su ./library/mps_reader.cyclo ./library/mps_reader.d ./library/mps_reader.o ./library/mps_reader.su ./library/mps_trace.cyclo ./library/mps_trace.d ./library/mps_trace.o ./library/mps_trace.su ./library/nist_kw.cyclo ./library/nist_kw.d ./library/nist_kw.o ./library/nist_kw.su ./library/oid.cyclo ./library/oid.d ./library/oid.o ./library/oid.su ./library/padlock.cyclo ./library/padlock.d ./library/padlock.o ./library/padlock.su ./library/pem.cyclo ./library/pem.d ./library/pem.o ./library/pem.su ./library/pk.cyclo ./library/pk.d ./library/pk.o ./library/pk.su ./library/pk_wrap.cyclo ./library/pk_wrap.d ./library/pk_wrap.o ./library/pk_wrap.su ./library/pkparse.cyclo ./library/pkparse.d ./library/pkparse.o ./library/pkparse.su ./library/pkwrite.cyclo ./library/pkwrite.d ./library/pkwrite.o ./library/pkwrite.su ./library/platform.cyclo ./library/platform.d ./library/platform.o ./library/platform.su ./library/platform_util.cyclo ./library/platform_util.d ./library/platform_util.o ./library/platform_util.su ./library/ripemd160.cyclo ./library/ripemd160.d ./library/ripemd160.o ./library/ripemd160.su ./library/sha1.cyclo ./library/sha1.d ./library/sha1.o ./library/sha1.su ./library/sha256.cyclo ./library/sha256.d ./library/sha256.o ./library/sha256.su ./library/sha512.cyclo ./library/sha512.d ./library/sha512.o ./library/sha512.su

.PHONY: clean-library

