scheduler

a simplified parallel transaction scheduler in c, inspired by solana's banking stage. transactions are prioritized by fees, account conflicts are detected using locks, and non-conflicting transactions are executed in parallel through a thread pool.

<img width="1065" height="614" alt="image" src="https://github.com/user-attachments/assets/c06ff0c6-e008-4bfe-add5-fc65ecf4038e" />

credits

- [cJSON](https://github.com/DaveGamble/cJSON) by Dave Gamble, vendored in `thirdparty/`
