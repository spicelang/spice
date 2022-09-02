; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Letter__string = type { ptr }

@0 = private unnamed_addr constant [11 x i8] c"No content\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1

define internal ptr @_mf__Letter__getContent(ptr %0) {
entry.l5:
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %result = alloca ptr, align 8
  %1 = load ptr, ptr %this, align 8
  %2 = getelementptr inbounds %_s__Letter__string, ptr %1, i32 0, i32 0
  %3 = load ptr, ptr %2, align 8
  ret ptr %3
}

define internal void @_mp__Letter__setContent__string(ptr %0, ptr %1) {
entry.l9:
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %text = alloca ptr, align 8
  store ptr %1, ptr %text, align 8
  %2 = load ptr, ptr %text, align 8
  %3 = load ptr, ptr %this, align 8
  %4 = getelementptr inbounds %_s__Letter__string, ptr %3, i32 0, i32 0
  store ptr %2, ptr %4, align 8
  ret void
}

define i32 @main() {
entry.l13:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca %_s__Letter__string, align 8
  %1 = getelementptr inbounds %_s__Letter__string, ptr %0, i32 0, i32 0
  store ptr @0, ptr %1, align 8
  %2 = alloca ptr, align 8
  store ptr @1, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  call void @_mp__Letter__setContent__string(ptr %0, ptr %3)
  %4 = alloca i1, align 1
  store i1 true, ptr %4, align 1
  %5 = call ptr @_mf__Letter__getContent(ptr %0)
  %6 = alloca ptr, align 8
  store ptr %5, ptr %6, align 8
  %7 = load ptr, ptr %6, align 8
  %8 = call i32 (ptr, ...) @printf(ptr @2, ptr %7)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

declare i32 @printf(ptr, ...)
