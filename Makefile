##
## EPITECH PROJECT, 2025
## myftp
## File description:
## Makefile
##

BUILD_DIR = build
CMAKE = cmake
MAKE = make

all: $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)
	cp $(BUILD_DIR)/myftp .

$(BUILD_DIR):
	$(CMAKE) -S . -B $(BUILD_DIR)

clean:
	$(MAKE) -C $(BUILD_DIR) clean

fclean:
	rm -rf $(BUILD_DIR)
	rm -f myftp

re: fclean all