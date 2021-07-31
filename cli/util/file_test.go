package util

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// -------------------------------------------- FileExists ---------------------------------------------

func TestFileExists_Success(t *testing.T) {
	result := FileExists("../test-files/test.txt")
	assert.True(t, result)
}

func TestFileExists_Failure(t *testing.T) {
	result := FileExists("../test-files/non-existing.txt")
	assert.False(t, result)
}
