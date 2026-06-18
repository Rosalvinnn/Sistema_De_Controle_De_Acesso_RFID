📖 Descrição

Este projeto consiste em um sistema de controle de acesso utilizando tecnologia RFID e Arduino. O objetivo é permitir a abertura automática de uma porta ou portão por meio da aproximação de um cartão ou chaveiro RFID previamente cadastrado.

Quando uma tag autorizada é detectada, o sistema libera o acesso acionando um motor e sinaliza a autorização através de um LED verde. Caso a tag não esteja cadastrada, o acesso é negado e um LED vermelho é acionado.

O projeto foi desenvolvido com foco em acessibilidade, automação e segurança.

🎯 Objetivos
Controlar o acesso de usuários autorizados.
Automatizar a abertura e fechamento de portas.
Utilizar RFID para identificação rápida e segura.
Fornecer sinalização visual de acesso autorizado ou negado.
Criar uma solução de baixo custo utilizando Arduino.
⚙️ Componentes Utilizados
Arduino Uno
Módulo RFID MFRC522
Cartões/Tags RFID
Ponte H L298N
Motor DC 775
LEDs (verde e vermelho)
Resistores 220 Ω
Fonte de alimentação
Protoboard
Jumpers
Estrutura física da porta em MDF
🔌 Funcionamento
O usuário aproxima um cartão RFID do leitor.
O Arduino lê o UID da tag.
O sistema verifica se a tag está cadastrada.
Se autorizada:
Acende o LED verde.
Aciona o motor para abrir a porta.
Aguarda alguns segundos.
Fecha a porta automaticamente.
Se não autorizada:
Acende o LED vermelho.
O acesso é negado.
🛠️ Diagrama de Ligações
RFID MFRC522
RFID	Arduino
SDA	10
SCK	13
MOSI	11
MISO	12
RST	9
3.3V	3.3V
GND	GND
LEDs
Componente	Pino
LED Verde	2
LED Vermelho	8
Ponte H L298N
L298N	Arduino
IN3	3
IN4	4
GND	GND
📋 Fluxograma
Início
   ↓
Aproxima Tag RFID
   ↓
Tag cadastrada?
 ┌─────┴─────┐
Sim         Não
 ↓           ↓
LED Verde   LED Vermelho
 ↓           ↓
Abrir Porta Acesso Negado
 ↓
Espera
 ↓
Fechar Porta
 ↓
Fim
🚀 Possíveis Melhorias
Cadastro de novos cartões pelo próprio sistema.
Integração com aplicativo móvel.
Registro de acessos em banco de dados.
Envio de notificações em tempo real.
Controle remoto via Wi-Fi utilizando ESP32.
Uso de sensor para detectar a posição da porta.
📷 Demonstração

Adicione aqui imagens do projeto:

/docs/imagens/projeto.jpg
/docs/imagens/montagem.jpg
/docs/imagens/porta-aberta.jpg
👥 Equipe
Nome do Integrante 1
Nome do Integrante 2
Nome do Integrante 3
Nome do Integrante 4
📄 Licença

Este projeto foi desenvolvido para fins acadêmicos e educacionais.
