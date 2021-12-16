package util

import "os"

// IsDockerized checks if Spice runs in a dockerized environment
func IsDockerized() bool {
	return os.Getenv("SPICE_DOCKERIZED") == "1"
}
