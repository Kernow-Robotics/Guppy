import hid

# Vendor and Product IDs for Xbox One controller
vendor_id = 0x045e  # Xbox vendor ID
product_id = 0x02d1  # Xbox product ID

try:
    # Find Xbox One controller by specifying vid/pid
    xbox_controller = hid.device()
    xbox_controller.open(vendor_id, product_id)
    print("Xbox One controller connected")

    while True:
        try:
            # Read input reports from the controller
            data = xbox_controller.read(64)
            if data:
                print(data)  # Display the input data
        except KeyboardInterrupt:
            break

    xbox_controller.close()
except IOError as e:
    print(f"Error connecting to Xbox controller: {e}")
