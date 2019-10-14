# Servidor Sts Exemplo

Requests:
    1. Geração do token:
http://localhost:8080/token
    2. Validação de token:
http://localhost:8080/token_info?token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJVU0VSX0lEIjoiMSIsInVzZXJuYW1lIjoidGVudGF0aXZhZmMifQ.NN9MXTfabh_tf9VeJbHoc7tGNd6fSf4jTfThSNkTBaY
    3. Teste de autorização:
http://localhost:8080/authorized_hello