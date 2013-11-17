mkdir runs
mkdir runs/cache_size
mkdir runs/cache_assoc
mkdir runs/cache_block_size


./smp_cache 262144 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MSI_256K
./smp_cache 524288 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MSI_512K
./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MSI_1M
./smp_cache 2097152 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MSI_2M

./smp_cache 262144 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MSI_256K
./smp_cache 524288 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MSI_512K
./smp_cache 1048576 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MSI_1M
./smp_cache 2097152 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MSI_2M

./smp_cache 262144 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MESI_256K
./smp_cache 524288 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MESI_512K
./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MESI_1M
./smp_cache 2097152 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MESI_2M

./smp_cache 262144 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MESI_256K
./smp_cache 524288 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MESI_512K
./smp_cache 1048576 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MESI_1M
./smp_cache 2097152 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MESI_2M

./smp_cache 262144 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MOESI_256K
./smp_cache 524288 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MOESI_512K
./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MOESI_1M
./smp_cache 2097152 8 64 16 0 CGaw > runs/cache_size/CGaw_VALIDATION_MOESI_2M

./smp_cache 262144 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MOESI_256K
./smp_cache 524288 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MOESI_512K
./smp_cache 1048576 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MOESI_1M
./smp_cache 2097152 8 64 16 0 CGad > runs/cache_size/CGad_VALIDATION_MOESI_2M






./smp_cache 1048576 4 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MSI_4
./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MSI_8
./smp_cache 1048576 16 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MSI_16

./smp_cache 1048576 4 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MSI_4
./smp_cache 1048576 8 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MSI_8
./smp_cache 1048576 16 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MSI_16

./smp_cache 1048576 4 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MESI_4
./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MESI_8
./smp_cache 1048576 16 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MESI_16

./smp_cache 1048576 4 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MESI_4
./smp_cache 1048576 8 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MESI_8
./smp_cache 1048576 16 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MESI_16

./smp_cache 1048576 4 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MOESI_4
./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MOESI_8
./smp_cache 1048576 16 64 16 0 CGaw > runs/cache_assoc/CGaw_VALIDATION_MOESI_16

./smp_cache 1048576 4 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MOESI_4
./smp_cache 1048576 8 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MOESI_8
./smp_cache 1048576 16 64 16 0 CGad > runs/cache_assoc/CGad_VALIDATION_MOESI_16






./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MSI_64
./smp_cache 1048576 8 128 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MSI_128
./smp_cache 1048576 8 256 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MSI_256

./smp_cache 1048576 8 64 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MSI_64
./smp_cache 1048576 8 128 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MSI_128
./smp_cache 1048576 8 256 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MSI_256

./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MESI_64
./smp_cache 1048576 8 128 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MESI_128
./smp_cache 1048576 8 256 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MESI_256

./smp_cache 1048576 8 64 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MESI_64
./smp_cache 1048576 8 128 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MESI_128
./smp_cache 1048576 8 256 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MESI_256

./smp_cache 1048576 8 64 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MOESI_64
./smp_cache 1048576 8 128 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MOESI_128
./smp_cache 1048576 8 256 16 0 CGaw > runs/cache_block_size/CGaw_VALIDATION_MOESI_256

./smp_cache 1048576 8 64 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MOESI_64
./smp_cache 1048576 8 128 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MOESI_128
./smp_cache 1048576 8 256 16 0 CGad > runs/cache_block_size/CGad_VALIDATION_MOESI_256



zip -r runs.zip runs
