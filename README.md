# imaqliq-test-task
Сервер - демон, работает в фоне.\n
Клиент отправляет текст из файла серверу, а сервер сохраняет этот текст у себя в файл.
Сборщик - CMake\n
Тесты - GTest\n
Для запуска в терминале необходимо ввести команды:
./server -p <port> -f <output_file>
./client -p <port> -f <input_file>
