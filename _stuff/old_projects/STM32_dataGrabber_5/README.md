# dataGrabber

## Etapas do desenvolvimento do firmware

### Requisitos mínimos

- [ ] Configuração básica do sistema
  - [x] Projeto no STM32CubeMX
  - [x] Projeto no Visual Studio Code
  - [x] LED
  - [x] Porta de DEBUG
  - [x] Debug usando a biblioteca FunSAPE
  - [X] Debug usando comunicação com o computador (UART3 + FT232)
  - [X] Cronômetro para temporização de eventos/processos
  - [ ] Configuração do VS code para depuração do código

- [ ] Configuração de periféricos do microcontrolador
  - [X] ADC1
  - [X] ADC3
  - [X] TIMER3
  - [X] TIMER8
  - [ ] I2C1
  - [ ] I2C2
  - [ ] SPI1
  - [ ] SPI2
  - [ ] UART2
  - [X] UART3

- [X] Encoder
  - [X] Detecção da direção do giro
  - [X] Detecção do pressionamento do botão

- [X] ECG (AD8232) / RESP (Faixa Elástica)
  - [X] Tratamento da interrupção do ADC para transferência dos dados para um buffer circular

- [ ] PPG (MAX30102)
  - [ ] Configuração do sensor
  - [ ] Leitura dos dados do sensor e transferência para um buffer circular

- [ ] RESP (MAX30102)
  - [ ] Configuração do sensor
  - [ ] Leitura dos dados do sensor e transferência para um buffer circular

- [ ] RESP (MPU9250)
  - [ ] Configuração do sensor
  - [ ] Leitura dos dados do sensor e transferência para um buffer circular

- [X] Cartão SD
  - [X] Configuração da biblioteca FatFS
  - [X] Montagem do cartão SD
  - [X] Busca pelo último arquivo gerado
  - [X] Criação do arquivo da aquisição
  - [X] Escrita do cabeçalho do arquivo
  - [X] Abertura do arquivo da aquisição para concatenação dos dados
  - [X] Escrita dos dados no cartão SD

- [ ] Empacotamento dos dados
  - [ ] Criação do pacote de dados com as amostras adquiridas
  - [ ] Transferência do pacote de dados para o buffer circular do cartão SD

- [ ] Bluetooth (HC-05)
  - [ ] Configuração do transmissor
  - [ ] Transmissão dos dados usando UART
  - [ ] Transmissão dos dados usando SPI

- [ ] Alimentação
  - [X] Leitura das tensões do sistema
  - [X] Cálculo dos valores das tensões em milivolts
  - [ ] Monitoramento do estado de carga das baterias

- [ ] Temporização
  - [ ] ECG (AD8232) - tempo para transferência dos dados para o buffer circular
  - [ ] PPG (MAX30102) - tempo para comunicação e transferência dos dados para o buffer circular
  - [ ] RESP (MAX30102) - tempo para comunicação e transferência dos dados para o buffer circular
  - [ ] RESP (MPU9250) - tempo para comunicação e transferência dos dados para o buffer circular
  - [ ] Cartão SD - tempos médio e máximo para escrita no cartão em diferentes frequências de SCLK
  - [ ] Cartão SD - tempos médio e máximo para escrita no cartão em diferentes cartões SD
  - [ ] Empacotamento - tempo de criação do pacote e transferência para o buffer circular
  - [ ] Bluetooth (HC-05) - tempo para transmissão dos dados

### Requisitos adicionais

- [ ] Configuração de periféricos do microcontrolador
  - [ ] RTC
  - [ ] UART1
  - [ ] USB

- [ ] RTC (DS1307)
  - [ ] Configuração do dispositivo
  - [ ] Verificar se a data/hora estão válidas na inicialização

- [ ] Display TFT
  - [ ] Tela de inicialização e auto-teste do sistema
  - [ ] Menu para seleção/configuração da aquisição
  - [ ] Apresentação da forma de onda de ECG na tela
  - [ ] Apresentação da forma de onda de PPG na tela
  - [ ] Apresentação da forma de onda de RESP na tela

- [ ] Bluetooth (BLE)
  - [ ] Configuração do transmissor
  - [ ] Transmissão dos dados usando UART
  - [ ] Transmissão dos dados usando SPI

- [ ] ECG (ADAS1000)
  - [ ] Verificar possibilidade de implementação

- [ ] EEPROM (AT24C08)
  - [ ] Verificar possibilidade de implementação

- [ ] Temporização
  - [ ] Display TFT - tempo para comunicação com o display
  - [ ] Bluetooth (BLE) - tempo para transmissão dos dados
