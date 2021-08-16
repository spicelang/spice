package util

import (
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

// ----------------------------------------- TestIsDockerized ------------------------------------------

func TestIsDockerized(t *testing.T) {
	os.Setenv("SPICE_DOCKERIZED", "1")
	assert.True(t, IsDockerized())
	os.Setenv("SPICE_DOCKERIZED", "0")
	assert.False(t, IsDockerized())
}
