# Makefile задает правила сборки проекта

# Название проекта
# Влияет на то, как будет называться файл прошивки
APPLICATION = button

# Путь, по которому располагается корень исходников RIOT
# Путь задается относительно текущей папки. 
RIOTBASE ?= $(CURDIR)/../../RIOT

# Название платы, для которой происходит сборка проекта
BOARD ?= stm32f0discovery

# Включение дополнительного кода проверки безопасности
DEVELHELP = 1

# Подключение модуля системного таймера
USEMODULE += xtimer

# Запрос функции таймера
FEATURES_REQUIRED += periph_timer

# Запрос функции GPIO
FEATURES_REQUIRED += periph_gpio

# Запрос функции прерываний от GPIO
FEATURES_REQUIRED += periph_gpio_irq

# Подлючение другого Makefile
include $(RIOTBASE)/Makefile.include