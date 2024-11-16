#!/bin/bash

# Define the name of the client program to terminate
CLIENT_PROGRAM="client"

echo "Attempting to terminate any running or idle '$CLIENT_PROGRAM' processes..."

# Find the process IDs (PIDs) of the client program
PIDS=$(pgrep -x $CLIENT_PROGRAM)

if [ -z "$PIDS" ]; then
  echo "No '$CLIENT_PROGRAM' processes are running. No action needed."
else
  echo "Found '$CLIENT_PROGRAM' process(es) with PID(s): $PIDS"
  echo "Attempting to terminate process(es)..."

  # Try to terminate the processes gracefully
  kill $PIDS 2>/dev/null
  sleep 1

  # Check if the processes are still running
  for PID in $PIDS; do
    if ps -p $PID > /dev/null 2>&1; then
      echo "Process $PID did not terminate gracefully. Forcing termination..."
      kill -9 $PID 2>/dev/null
    fi
  done

  echo "Processes for '$CLIENT_PROGRAM' have been terminated."
fi

echo "All '$CLIENT_PROGRAM' processes are now stopped."
