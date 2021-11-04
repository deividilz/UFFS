module sinal (
    input [11:0] Ponto1X,
    input [11:0] Ponto1Y,

    input [11:0] Ponto2X,
    input [11:0] Ponto2Y,

    input [11:0] PontoTX,
    input [11:0] PontoTY,

    output sinal
);

    // DECLARAÇÃO DOS FIOS
    wire signed [22:0] Mult1;
    wire signed [22:0] Mult2;

    wire signed [11:0] Sub1;
    wire signed [11:0] Sub2;
    wire signed [11:0] Sub3;
    wire signed [11:0] Sub4;
    wire signed [22:0] Sub5;
    

    // LIGAÇÃO DOS FIOS
    assign Sub1 = PontoTX - Ponto2Y;
    assign Sub2 = Ponto1Y - Ponto2Y;
    assign Sub3 = Ponto1X - Ponto2X;
    assign Sub4 = PontoTY - Ponto2Y;

    assign Mult1 = Sub1 * Sub2;
    assign Mult2 = Sub3 * Sub4;

    assign Sub5 = Mult1 - Mult2;

    assign sinal = (Sub5 >= 0) ? 1 : 0; //Verificacao caso o resultado for maior ou igual a zero

endmodule


module Triangulo (
    input [11:0] Ponto1X,
    input [11:0] Ponto1Y,

    input [11:0] Ponto2X,
    input [11:0] Ponto2Y,

    input [11:0] Ponto3X,
    input [11:0] Ponto3Y,

    input [11:0] PontoTX,
    input [11:0] PontoTY,

    output dentro
    );

    wire sinal1;
    wire sinal2;
    wire sinal3;

    assign dentro = (sinal1 == 1 && sinal2 == 1 && sinal3 == 1) ? 1:0;

    sinal S1(Ponto1X, Ponto1Y, Ponto2X, Ponto2Y, PontoTX, PontoTY, sinal1);
    sinal S2(Ponto2X, Ponto2Y, Ponto3X, Ponto3Y, PontoTX, PontoTY, sinal2);
    sinal S3(Ponto3X, Ponto3Y, Ponto1X, Ponto1Y, PontoTX, PontoTY, sinal3);

endmodule

module Teste;
    reg [11:0] Ponto1X;
    reg [11:0] Ponto1Y;

    reg [11:0] Ponto2X;
    reg [11:0] Ponto2Y;

    reg [11:0] Ponto3X;
    reg [11:0] Ponto3Y;

    reg [11:0] PontoTX;
    reg [11:0] PontoTY;

    wire Dentro;

    Triangulo A(Ponto1X, Ponto1Y, Ponto2X, Ponto2Y, Ponto3X, Ponto3Y, PontoTX, PontoTY, Dentro);

    initial
        begin
           $dumpvars(0,A);
           #1

           Ponto1X <= 10;
           Ponto1Y <= 10;
           Ponto2X <= 30;
           Ponto2Y <= 10;
           Ponto3X <= 20;
           Ponto3Y <= 30;
           PontoTX <= 15;
           PontoTY <= 15;

           #1
           PontoTX <= 15;
           PontoTY <= 15;
           #1
           PontoTX <= 9;
           PontoTY <= 15;
           #1
           PontoTX <= 10;
           PontoTY <= 11;
           #1
           PontoTX <= 30;
           PontoTY <= 11;
           #40
           $finish;
        end
endmodule