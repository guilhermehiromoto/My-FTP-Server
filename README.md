# My FTP Server

![image](https://user-images.githubusercontent.com/48016991/121280837-47892600-c8ad-11eb-87f1-e0bf338417c3.png)


Esta é uma implementação simples do funcionamento de um servidor FTP para compartilhamento de arquivos feito em C. Você pode subir arquivos com o comando put e baixar arquivos com o comando get.

## Membros

* Caio Marcos Chaves Viana - 11208217
* Guilherme Amaral Hiromoto - 11218959
* Maria Fernanda Lucio de Mello - 11320860
* Paulo Matana da Rocha - 10892676
* Victor Rodrigues Russo - 11218855

## Softwares Necessários

O funcionamento da comunicação cliente-servidor foi testada utilizando:

- OS Debian GNU/Linux bullseye/sid x8
- Kernel: 5.9.0-4-amd64
- Compiler: gcc (Debian 10.2.1-1) 10.2.1 20201207

## Instalando o Software

Para instalar o software basta rodar o seguinte comando em seu terminal:

```
git clone https://github.com/guilhermehiromoto/TCP-Server.git
```

## Executando

Para compilar o código basta navegar até a pasta raiz do repositório clonado e executar:

```
make all
```

Para compilar apenas o código do server basta executar:

```
make server
```

Para compilar apenas o código do client basta executar:

```
make client
```

Para executar o código do servidor basta executar:

```
make runServer
```

Para executar o cliente basta executar:

```
make runClient
```

## Comandos

É importante notar que os arquivos a serem compartilhados devem estar dentro da pasta files do client ou da pasta files de serves.No cliente podemos subir ou baixar um arquivo com os seguintes comandos:

```
get file1.txt
```

```
put img.jpg
```

E podemos ainda sair:

```
exit
```
