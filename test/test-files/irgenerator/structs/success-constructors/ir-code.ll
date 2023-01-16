; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Vector__bool_string = type { i1, ptr }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"Test string\00", align 1
@anon.string.1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1
@anon.string.2 = private unnamed_addr constant [16 x i8] c"Another message\00", align 1
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Message: %s\0A\00", align 1

define private void @_mp__Vector__void__ctor(ptr noundef nonnull %0) {
  %this = alloca ptr, align 8
  %msg = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr @anon.string.0, ptr %msg, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %__Vector__bool_string, ptr %2, i32 0, i32 0
  store i1 false, ptr %3, align 1
  %4 = load ptr, ptr %this, align 8
  %5 = getelementptr inbounds %__Vector__bool_string, ptr %4, i32 0, i32 1
  %6 = load ptr, ptr %msg, align 8
  store ptr %6, ptr %5, align 8
  ret void
}

define private void @_mp__Vector__void__ctor__string(ptr noundef nonnull %0, ptr %1) {
  %this = alloca ptr, align 8
  %msg = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %msg, align 8
  %3 = load ptr, ptr %this, align 8
  %4 = getelementptr inbounds %__Vector__bool_string, ptr %3, i32 0, i32 0
  store i1 false, ptr %4, align 1
  %5 = load ptr, ptr %this, align 8
  %6 = getelementptr inbounds %__Vector__bool_string, ptr %5, i32 0, i32 1
  %7 = load ptr, ptr %msg, align 8
  store ptr %7, ptr %6, align 8
  ret void
}

define private ptr @_mf__Vector__string__test(ptr noundef nonnull %0) {
  %result = alloca ptr, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  ret ptr @anon.string.1
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %vec = alloca %__Vector__bool_string, align 8
  %1 = alloca %__Vector__bool_string, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__Vector__void__ctor(ptr %vec)
  %2 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 0
  %3 = load i1, ptr %2, align 1
  %4 = zext i1 %3 to i32
  %5 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 1
  %6 = load ptr, ptr %5, align 8
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %4, ptr %6)
  call void @_mp__Vector__void__ctor__string(ptr %1, ptr @anon.string.2)
  call void @llvm.memcpy.p0.p0.i64(ptr %vec, ptr %1, i64 16, i1 false)
  %8 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 0
  %9 = load i1, ptr %8, align 1
  %10 = zext i1 %9 to i32
  %11 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 1
  %12 = load ptr, ptr %11, align 8
  %13 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %10, ptr %12)
  %14 = call ptr @_mf__Vector__string__test(ptr %vec)
  %15 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %14)
  %16 = load i32, ptr %result, align 4
  ret i32 %16
}

declare i32 @printf(ptr noundef, ...)

; Function Attrs: argmemonly nocallback nofree nounwind willreturn
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { argmemonly nocallback nofree nounwind willreturn }
