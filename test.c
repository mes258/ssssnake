int main(int argc, char *argv[]) {
  // Pthread setup would be here
  sem_init(&mutex, 0, 1);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, 4);

  int i, in = 0;
  for(i = 0; i < 10; i++) {
    sem_wait(&empty);
    sem_wait(&mutex);
    buffer[in] = i;
    sem_post(&mutex);
    sem_post(&full);
    in = (in + 1) % 5;
    printf("Parent: %d\n", i);
    fflush(stdout);
  }
  // Pthread cleanup would be here
 }

 void *child() {
  sleep(3); // Sleep 3 seconds
  printf("Child is awake.\n");
  fflush(stdout);
  int i, out = 0;
  for(i = 0; i < 10; i++) {
    sem_wait(&full);
    sem_wait(&mutex);
    int data = buffer[out];
    printf("Child: %d\n", data);
    fflush(stdout);
    sem_post(&mutex);
    out = (out+1) % 5;
  }
  pthread_exit(0);
 }
