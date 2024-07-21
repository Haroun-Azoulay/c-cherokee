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
## 💻 Exemple Commands 

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

## ❤️ Support  
A simple star to this project repo is enough to keep me motivated on this project for days. If you find your self very much excited with this project let me know with a tweet.

## 🙇 Author
#### Cedric Chau
- Github: [@cchaud8](https://github.com/cchau8)
#### Haroun Azoulay
- Github: [@Haroun-Azoulay](https://github.com/Haroun-Azoulay)
