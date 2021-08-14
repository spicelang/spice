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

// ---------------------------------------- GetObjectFileTree ------------------------------------------

func TestGetObjectFileTree(t *testing.T) {
	result := GetObjectFileTree("../test-files/object-dir")
	expected := []string{
		"../test-files/object-dir/objectFile1.o",
		"../test-files/object-dir/objectFile2.o",
		"../test-files/object-dir/nestedDir/objectFile1.o",
	}
	assert.ElementsMatch(t, expected, result)
}
