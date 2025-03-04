# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/03 10:38:15 by kipouliq          #+#    #+#              #
#    Updated: 2025/03/04 15:17:16 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = main

SRCS = ./main.c

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -O1

NO_LINKING = -c $< -o $@

CC = avr-gcc

DEVICE = atmega328p

### COMPILATION

ARCH = -mmcu=$(DEVICE) # specifying with architecture to compile for

CPU_FREQ = -DF_CPU=16000000UL # defining CPU Clock frequency

BAUD = -DBAUD=11500 # defining baud rate (transmission speed based on cpu clock frequency(bitrate / nb of bits per unit data))

### FLASHING

PARTNO = -p m328p # specify which microcontroller is being programmed (m328p == atmega328p)

PROGRAMMER = -c arduino # specify a programer for the flashing stage

PORT = -P /dev/ttyUSB0 # specify which port ; to change depending on your computer

FLASH_MEM = -U flash:w:$(NAME).hex:i # actually doing the programming ; syntax : -U <memtype>:r|w|v:<filename>[:format]

VERBOSE = -v # verbose output, for debugging

all: hex
	# flash

hex: $(NAME).elf $(NAME).bin

$(NAME).elf: $(OBJS)
	$(CC) $(ARCH) $(OBJS) -o $@

$(NAME).bin: $(NAME).elf	# converting .elf to .bin to .hex
	avr-objcopy -O binary $(NAME).elf $(NAME).bin
	avr-objcopy -O ihex $(NAME).elf $(NAME).hex
	rm $(NAME).o $(NAME).elf $(NAME).bin 

%.o:%.c
	$(CC) $(CFLAGS) $(ARCH) $(CPU_FREQ) $(BAUD) $(NO_LINKING)

flash: hex
	avrdude $(PARTNO) $(PROGRAMMER) $(PORT) $(FLASH_MEM) $(VERBOSE)
	
clean: 
	rm -f *.o 
	rm -f ./$(NAME).hex
	rm -f ./$(NAME).bin

.PHONY: hex flash clean

# main.c 	-> all c files -> .o files (compiling stage)
#			-> main.elf (linking stage)
#			-> objcopy create main.hex # 