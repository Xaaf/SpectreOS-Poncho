echo "+-----> Changing directory to ../gnu-efi"
cd ../gnu-efi

echo "+-----> Running 'make bootloader'..."
make bootloader

echo "+-----> Changing directory to ../kernel"
cd ../kernel

echo "+-----> Running 'make kernel'"
make kernel

echo "+-----> Running 'make buildimg'"
make buildimg

read -p "+-----> !!! Do you want to run the image?" yn
case $yn in
	[Yy]* ) make run; break;;
	[Nn]* ) echo "Not running the image. Exiting..."; exit;;
	* ) echo "No input received. Running the image..."; make run; break;;
esac