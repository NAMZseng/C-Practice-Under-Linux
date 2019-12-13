obj=player_demo.c uart.c
exec=demo

$(exec):$(obj)
	gcc $^ -o $@ `pkg-config --cflags --libs gtk+-2.0` -lpthread
clean:
	rm $(exec)
