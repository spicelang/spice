; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Letter__string = type { ptr }

@anon.string.0 = private unnamed_addr constant [11 x i8] c"No content\00", align 1
@anon.struct.0 = private unnamed_addr constant %__Letter__string { ptr @anon.string.0 }
@anon.string.1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@printf.str.0 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1

define private ptr @_mf__Letter__string__getContent(ptr noundef nonnull %0) {
  %result = alloca ptr, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %content = getelementptr inbounds %__Letter__string, ptr %2, i32 0, i32 0
  %3 = load ptr, ptr %content, align 8
  ret ptr %3
}

define private void @_mp__Letter__void__setContent__string(ptr noundef nonnull %0, ptr %1) {
  %this = alloca ptr, align 8
  %text = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %text, align 8
  %3 = load ptr, ptr %this, align 8
  %content = getelementptr inbounds %__Letter__string, ptr %3, i32 0, i32 0
  %4 = load ptr, ptr %text, align 8
  store ptr %4, ptr %content, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %letter = alloca %__Letter__string, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %letter, ptr @anon.struct.0, i64 8, i1 false)
  store %__Letter__string { ptr @anon.string.0 }, ptr %letter, align 8
  call void @_mp__Letter__void__setContent__string(ptr %letter, ptr @anon.string.1)
  %1 = call ptr @_mf__Letter__string__getContent(ptr %letter)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
