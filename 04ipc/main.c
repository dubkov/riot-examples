// Подключение библиотек
#include "thread.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "msg.h"

// Выделение памяти под стеки двух тредов
char thread_one_stack[THREAD_STACKSIZE_DEFAULT];
char thread_two_stack[THREAD_STACKSIZE_DEFAULT];

// Глобально объявляется структура, которая будет хранить PID (идентификатор треда)
// Нам нужно знать этот идентификатор, чтобы посылать сообщения в тред
static kernel_pid_t thread_one_pid;

// Обработчик прерывания с кнопки
void btn_handler(void *arg)
{
  // Прием аргументов из главного потока
  (void)arg;
  // Создание объекта для хранения сообщения
  msg_t msg;
  // Отправка сообщения в тред по его PID
  // Сообщение остается пустым, поскольку нам сейчас важен только сам факт его наличия, и данные мы не передаем
	msg_send(&msg, thread_one_pid);
}

// Первый тред
void *thread_one(void *arg)
{
  // Прием аргументов из главного потока
  (void) arg;
  // Создание объекта для хранения сообщения 
  msg_t msg;
  // Инициализация пина PC8 на выход
  gpio_init(GPIO_PIN(PORT_C,8),GPIO_OUT);
  while(1){
    // Ждем, пока придет сообщение
    // msg_receive - это блокирующая операция, поэтому задача зависнет в этом месте, пока не придет сообщение
    // а бывает еще неблокирующий прием сообщения - msg_try_receive
  	msg_receive(&msg);
    // Переключаем светодиод
    gpio_toggle(GPIO_PIN(PORT_C,8));
  }
  // Хотя код до сюда не должен дойти, написать возврат NULL все же обязательно надо    
  return NULL;
}

// Второй поток
void *thread_two(void *arg)
{

  // Прием аргументов из главного потока
  (void) arg;
  // Инициализация пина PC9 на выход
  gpio_init(GPIO_PIN(PORT_C,9),GPIO_OUT);
  // ВременнАя метка для отсчета времени сна
  xtimer_ticks32_t last_wakeup2 = xtimer_now();

  while(1){
    // Переключаем светодиод
  	gpio_toggle(GPIO_PIN(PORT_C,9));
    // Задача засыпает на 333333 микросекунды
   	xtimer_periodic_wakeup(&last_wakeup2, 333333);
  }    
  return NULL;
}


int main(void)
{
  // Инициализация прерывания с кнопки (подробнее в примере 02button)
	gpio_init_int(GPIO_PIN(PORT_A,0),GPIO_IN,GPIO_RISING,btn_handler,NULL);

  // Создение потоков (подробнее в примере 03threads)
  // Для первого потока мы сохраняем себе то, что возвращает функция thread_create,
  // чтобы потом пользоваться этим как идентификатором потока для отправки ему сообщений
  thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  thread_one, NULL, "thread_one");
  // обратите внимание, что у потоков разный приоритет
  thread_create(thread_two_stack, sizeof(thread_two_stack),
                  THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                  thread_two, NULL, "thread_two");

  while (1){

    }

    return 0;
}

/* 
  Задание 1: Добавьте в код подавление дребезга кнопки
  Задание 2: Сделайте так, чтобы из прерывания по кнопке в поток thread_one передавалось целое число,
              которое означает, сколько раз должен моргнуть светодиод после нажатия кнопки.
              Передать значение в сообщении можно через поле msg.content.value
              После каждого нажатия циклически инкрементируйте значение от 1 до 5.
  Задание 3: Добавьте прерывание по отпусканию кнопки. Сделайте из этого прерывания отправку сообщения
              в поток thread_two, которое будет передавать значение интервала между морганиями светодиода.

  Что поизучать: https://riot-os.org/api/group__core__msg.html
*/