# <p align="center">Cherokee-Http server c++</p>
  
Project of master is to implement a server conforming to a simplified version of the HTTP protocol on C++.

### Architecture about server Htpp

![Sequence Diagram](http://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/Haroun-Azoulay/c_cherokee/main/sequence_diagram.puml)
        
## 🛠️ Tech Stack

- [C++](https://isocpp.org/)

## 🧐 Features

- [Siege](https://www.joedog.org/siege-manual/)
- [Criterion](https://criterion.readthedocs.io/en/master/intro.html)

## 🛠️ Install Dependencies 

### Docker image building

```bash
docker build -t my_image:tag .
```

### Run the project with Docker

```bash
docker run --network host -ti my_image:tag
```

### Test the project with Docker

```bash
docker run -ti -e MODE=test my_image:tag
```
## 💻 Examples Commands 

You can use the following commands:

- GET request to retrieve index.html
```bash
curl http://localhost:8080/index.html
```

- POST request to create or update data.txt
```bash
curl -X POST -d 'This is some data' http://localhost:8080/data.txt
```

- PUT request to update data.txt
```bash
curl -X PUT -d 'This is updated data' http://localhost:8080/data.txt
```

DELETE request to remove data.txt
```bash
curl -X DELETE http://localhost:8080/data.txt
```

## 💻 Siege 

#### GET - 10 users - 1min

`siege -c 10 -t 1M http://localhost:8080`
```
        "transactions":                       430897,
        "availability":                       100.00,
        "elapsed_time":                        59.02,
        "data_transferred":                    36.57,
        "response_time":                        0.00,
        "transaction_rate":                  7300.86,
        "throughput":                           0.62,
        "concurrency":                          9.29,
        "successful_transactions":            430897,
        "failed_transactions":                     0,
        "longest_transaction":                  0.05,
        "shortest_transaction":                 0.00
```

#### GET - 100 users - 1min

`siege -c 100 -t 1M http://localhost:8080`
```
        "transactions":                       447840,
        "availability":                       100.00,
        "elapsed_time":                        59.97,
        "data_transferred":                    38.01,
        "response_time":                        0.01,
        "transaction_rate":                  7467.73,
        "throughput":                           0.63,
        "concurrency":                         98.33,
        "successful_transactions":            447841,
        "failed_transactions":                     0,
        "longest_transaction":                  7.85,
        "shortest_transaction":                 0.00
```

#### Post file txt - 100 users - 1min

`siege -c 50 -t 1M 'http://localhost:8080/test.txt POST "test dummy"'`
```
        "transactions":                       419179,
        "availability":                       100.00,
        "elapsed_time":                        59.52,
        "data_transferred":                    10.79,
        "response_time":                        0.01,
        "transaction_rate":                  7042.66,
        "throughput":                           0.18,
        "concurrency":                         48.94,
        "successful_transactions":            419180,
        "failed_transactions":                     0,
        "longest_transaction":                  2.07,
        "shortest_transaction":                 0.00
```

#### Post picture png - 100 users - 1min

`siege -c 100 -t 1M 'http://localhost:8080/test.png POST ./examples_files/picture.png'`

```
        "transactions":                       398727,
        "availability":                       100.00,
        "elapsed_time":                        59.13,
        "data_transferred":                    10.27,
        "response_time":                        0.01,
        "transaction_rate":                  6743.23,
        "throughput":                           0.17,
        "concurrency":                         98.58,
        "successful_transactions":            398728,
        "failed_transactions":                     0,
        "longest_transaction":                  3.08,
        "shortest_transaction":                 0.00
```

`siege -c 100 -t 1M  'http://localhost:8080/test.jpg POST ./examples_files/picture.jpg'`

 ```
        "transactions":                       359838,
        "availability":                       100.00,
        "elapsed_time":                        59.71,
        "data_transferred":                     9.27,
        "response_time":                        0.02,
        "transaction_rate":                  6026.43,
        "throughput":                           0.16,
        "concurrency":                         98.35,
        "successful_transactions":            359838,
        "failed_transactions":                     0,
        "longest_transaction":                  3.05,
        "shortest_transaction":                 0.00
```
## ❤️ Support  
A simple star to this project repo is enough to keep me motivated on this project for days. If you find your self very much excited with this project let me know with a tweet.

## 🙇 Author
#### Cedric Chau
- Github: [@cchaud8](https://github.com/cchau8)
#### Haroun Azoulay
- Github: [@Haroun-Azoulay](https://github.com/Haroun-Azoulay)
