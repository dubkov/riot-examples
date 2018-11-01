# название приложения
APPLICATION = adc
# плата, под которую выполняется сборка проекта
BOARD ?= stm32f0discovery
# путь до корня исходников RIOT
RIOTBASE ?= $(CURDIR)/../../RIOT
# требуем функцию АЦП
FEATURES_REQUIRED = periph_adc
# используем модуль системного таймера
USEMODULE += xtimer
# подключение следующего мэйкфайла
include $(RIOTBASE)/Makefile.include
