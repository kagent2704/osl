#!/bin/bash

# Set backup directory name
BACKUP_DIR="backup"
ARCHIVE_NAME="backup_$(date +%Y%m%d_%H%M%S).tar.gz"

# Step 1: Create the backup directory if it doesn't exist
mkdir -p "$BACKUP_DIR"

# Step 2: Copy all .txt files to the backup directory
cp *.txt "$BACKUP_DIR" 2>/dev/null

# Step 3: Compress the backup directory into a tar.gz archive
tar -czf "$ARCHIVE_NAME" "$BACKUP_DIR"

# Step 4: Print success message
echo "Backup complete!"
echo "All .txt files backed up to '$BACKUP_DIR' and archived as '$ARCHIVE_NAME'"
