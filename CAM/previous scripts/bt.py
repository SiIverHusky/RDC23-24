import bluetooth

print("Scanning for Bluetooth devices...")
devices = bluetooth.discover(lookup_names=True)
print("Found {} devices.".format(len(devices)))