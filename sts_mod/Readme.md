# Módulo Apache de Authorization em uma configuração de proxy reverso

### Pré Req.:

```sh
sudo apt-get install apache2-dev
sudo apt-get install apache2
```

### Comandos úteis

Compilação C:

```sh
gcc curll.c -lcurl -o curll
```

Compilação do módulo:

```sh
sudo apxs -i -a -c -o mod_sts.so -c mod_sts.c
```

Compilação do módulo com link para o curl:

```sh
sudo apxs -i -a -c -o mod_sts.so -c mod_sts.c -lcurl
```

Arquivo onde se configura o handler:

```sh
sudo vim /etc/apache2/apache2.conf
AddHandler sts-handler /authorized_hello
```

Locais onde o módulo é configurado:

```sh
ls /etc/apache2/mods-enabled/
sudo rm /etc/apache2/mods-enabled/sts.load
sudo rm /usr/lib/apache2/modules/mod_sts.so
sudo rm /usr/lib/apache2/modules/mod_sts.la
```

Restart do servidor:

```sh
systemctl restart apache2
```

Health check das configurações:

```sh
apache2ctl configtest
```

### Configuração do proxy reverso:

```sh
sudo a2enmod proxy
sudo a2enmod proxy_http
sudo a2enmod proxy_balancer
sudo a2enmod lbmethod_byrequests
sudo systemctl restart apache2.service
```


### Configuração do Proxy Reverso:

```sh
sudo vim /etc/apache2/sites-available/000-default.conf
```

Adicionar as linhas abaixo:

```sh
StsUrlValidation "http://localhost:8080/token_info?token=%s"
<VirtualHost *:80>
        ProxyPreserveHost On
        ProxyPass / http://127.0.0.1:8080/
        ProxyPassReverse / http://127.0.0.1:8080/
        SetHandler sts-handler
</VirtualHost>
```

### Chamadas:

Geração do token:

```sh
curl -X GET \
  http://localhost:8080/token
```

Acesso url restrita:
```sh
curl -X GET \
  http://localhost/authorized_hello \
  -H 'authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJVU0VSX0lEIjiMSIsInVzZXJuYW1lIjoidGVudGF0aXZhZmMifQ.NN9MXTfabh_tf9VeJbHoc7tGNd6fSf4jTfThSNkTBaY'
```

### Links úteis:

https://httpd.apache.org/docs/2.4/howto/reverse_proxy.html

https://www.digitalocean.com/community/tutorials/how-to-use-apache-as-a-reverse-proxy-with-mod_proxy-on-ubuntu-16-04

https://www.tirasa.net/en/blog/developing-custom-apache2-module