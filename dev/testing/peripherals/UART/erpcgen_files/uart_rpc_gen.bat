if not exist "./src/" mkdir ./src
if not exist "./py/" mkdir ./py

erpcgen.exe -g c -o ./src/ -v uart_rpc.erpc 
erpcgen.exe -g py -o ./py/ -v uart_rpc.erpc 