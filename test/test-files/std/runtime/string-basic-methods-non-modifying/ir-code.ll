; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"Hello!\00", align 1
@anon.string.3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.4 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 's1.isEmpty()' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!s2.isEmpty()' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's3.getLength() == 6' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 's4.getLength() == 12' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's3.getCapacity() == 12' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's4.getCapacity() == 24' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 's2.isFull()' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition '!s4.isFull()' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [4 x i8] c"ell\00", align 1
@anon.string.13 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's4.find(\22ell\22) == 1' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [5 x i8] c"Wort\00", align 1
@anon.string.15 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's4.find(\22Wort\22) == -1' evaluated to false.\00", align 1
@anon.string.16 = private unnamed_addr constant [2 x i8] c"H\00", align 1
@anon.string.17 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 's4.find(\22H\22) == 0' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [2 x i8] c"!\00", align 1
@anon.string.19 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's4.find(\22!\22) == 11' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [2 x i8] c" \00", align 1
@anon.string.21 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's4.find(\22 \22, 12) == -1' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@anon.string.23 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition '!s4.contains(\22abc\22)' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.25 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 's4.contains(\22Hello\22)' evaluated to false.\00", align 1
@anon.string.26 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.27 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's4.contains(\22World!\22)' evaluated to false.\00", align 1
@anon.string.28 = private unnamed_addr constant [4 x i8] c"o W\00", align 1
@anon.string.29 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's4.contains(\22o W\22)' evaluated to false.\00", align 1
@anon.string.30 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.31 = private unnamed_addr constant [74 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22Hello\22' evaluated to false.\00", align 1
@anon.string.32 = private unnamed_addr constant [3 x i8] c"o \00", align 1
@anon.string.33 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22o \22' evaluated to false.\00", align 1
@anon.string.34 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.35 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22World!\22' evaluated to false.\00", align 1
@anon.string.36 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.37 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22\22' evaluated to false.\00", align 1
@anon.string.38 = private unnamed_addr constant [11 x i8] c"llo World!\00", align 1
@anon.string.39 = private unnamed_addr constant [79 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22llo World!\22' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %s1 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %s3 = alloca %__String__charptr_long_long, align 8
  %s4 = alloca %__String__charptr_long_long, align 8
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %3 = alloca i1, align 1
  %sub = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %5 = alloca %__String__charptr_long_long, align 8
  %6 = alloca %__String__charptr_long_long, align 8
  %7 = alloca %__String__charptr_long_long, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__String__void__ctor__string(ptr %s1, ptr @anon.string.0)
  call void @_mp__String__void__ctor__string(ptr %s2, ptr @anon.string.1)
  call void @_mp__String__void__ctor__string(ptr %s3, ptr @anon.string.2)
  call void @_mp__String__void__ctor__string(ptr %s4, ptr @anon.string.3)
  %8 = call i1 @_mf__String__bool__isEmpty(ptr %s1)
  br i1 %8, label %assert.exit.L8, label %assert.then.L8, !prof !0

assert.then.L8:                                   ; preds = %0
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L8:                                   ; preds = %0
  %10 = call i1 @_mf__String__bool__isEmpty(ptr %s2)
  %11 = xor i1 %10, true
  store i1 %11, ptr %1, align 1
  br i1 %11, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %assert.exit.L8
  %12 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %assert.exit.L8
  %13 = call i64 @_mf__String__long__getLength(ptr %s3)
  %14 = icmp eq i64 %13, 6
  br i1 %14, label %assert.exit.L10, label %assert.then.L10, !prof !0

assert.then.L10:                                  ; preds = %assert.exit.L9
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L9
  %16 = call i64 @_mf__String__long__getLength(ptr %s4)
  %17 = icmp eq i64 %16, 12
  br i1 %17, label %assert.exit.L11, label %assert.then.L11, !prof !0

assert.then.L11:                                  ; preds = %assert.exit.L10
  %18 = call i32 (ptr, ...) @printf(ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L10
  %19 = call i64 @_mf__String__long__getCapacity(ptr %s3)
  %20 = icmp eq i64 %19, 12
  br i1 %20, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %assert.exit.L11
  %21 = call i32 (ptr, ...) @printf(ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L11
  %22 = call i64 @_mf__String__long__getCapacity(ptr %s4)
  %23 = icmp eq i64 %22, 24
  br i1 %23, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L12
  %24 = call i32 (ptr, ...) @printf(ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L12
  %25 = call i1 @_mf__String__bool__isFull(ptr %s2)
  br i1 %25, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %27 = call i1 @_mf__String__bool__isFull(ptr %s4)
  %28 = xor i1 %27, true
  store i1 %28, ptr %2, align 1
  br i1 %28, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L14
  %29 = call i32 (ptr, ...) @printf(ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %30 = call i64 @_mf__String__long__find__string(ptr %s4, ptr @anon.string.12)
  %31 = icmp eq i64 %30, 1
  br i1 %31, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %assert.exit.L15
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L15
  %33 = call i64 @_mf__String__long__find__string(ptr %s4, ptr @anon.string.14)
  %34 = icmp eq i64 %33, -1
  br i1 %34, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L16
  %35 = call i32 (ptr, ...) @printf(ptr @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L16
  %36 = call i64 @_mf__String__long__find__string(ptr %s4, ptr @anon.string.16)
  %37 = icmp eq i64 %36, 0
  br i1 %37, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %38 = call i32 (ptr, ...) @printf(ptr @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  %39 = call i64 @_mf__String__long__find__string(ptr %s4, ptr @anon.string.18)
  %40 = icmp eq i64 %39, 11
  br i1 %40, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %assert.exit.L18
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L18
  %42 = call i64 @_mf__String__long__find__string_int(ptr %s4, ptr @anon.string.20, i32 12)
  %43 = icmp eq i64 %42, -1
  br i1 %43, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L19
  %44 = call i32 (ptr, ...) @printf(ptr @anon.string.21)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L19
  %45 = call i1 @_mf__String__bool__contains__string(ptr %s4, ptr @anon.string.22)
  %46 = xor i1 %45, true
  store i1 %46, ptr %3, align 1
  br i1 %46, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L20
  %47 = call i32 (ptr, ...) @printf(ptr @anon.string.23)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  %48 = call i1 @_mf__String__bool__contains__string(ptr %s4, ptr @anon.string.24)
  br i1 %48, label %assert.exit.L22, label %assert.then.L22, !prof !0

assert.then.L22:                                  ; preds = %assert.exit.L21
  %49 = call i32 (ptr, ...) @printf(ptr @anon.string.25)
  call void @exit(i32 1)
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  %50 = call i1 @_mf__String__bool__contains__string(ptr %s4, ptr @anon.string.26)
  br i1 %50, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L22
  %51 = call i32 (ptr, ...) @printf(ptr @anon.string.27)
  call void @exit(i32 1)
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L22
  %52 = call i1 @_mf__String__bool__contains__string(ptr %s4, ptr @anon.string.28)
  br i1 %52, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L23
  %53 = call i32 (ptr, ...) @printf(ptr @anon.string.29)
  call void @exit(i32 1)
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L23
  %54 = call %__String__charptr_long_long @_mf__String__String__getSubstring__int_long(ptr %s4, i32 0, i64 5)
  store %__String__charptr_long_long %54, ptr %sub, align 8
  store %__String__charptr_long_long %54, ptr %sub, align 8
  %55 = call ptr @_mf__String__string__getRaw(ptr %sub)
  %56 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %55, ptr @anon.string.30)
  br i1 %56, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L24
  %57 = call i32 (ptr, ...) @printf(ptr @anon.string.31)
  call void @exit(i32 1)
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L24
  %58 = call %__String__charptr_long_long @_mf__String__String__getSubstring__long_long(ptr %s4, i64 4, i64 2)
  store %__String__charptr_long_long %58, ptr %4, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %sub, ptr %4, i64 24, i1 false)
  %59 = call ptr @_mf__String__string__getRaw(ptr %sub)
  %60 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %59, ptr @anon.string.32)
  br i1 %60, label %assert.exit.L28, label %assert.then.L28, !prof !0

assert.then.L28:                                  ; preds = %assert.exit.L26
  %61 = call i32 (ptr, ...) @printf(ptr @anon.string.33)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L26
  %62 = call %__String__charptr_long_long @_mf__String__String__getSubstring__int(ptr %s4, i32 6)
  store %__String__charptr_long_long %62, ptr %5, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %sub, ptr %5, i64 24, i1 false)
  %63 = call ptr @_mf__String__string__getRaw(ptr %sub)
  %64 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %63, ptr @anon.string.34)
  br i1 %64, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L28
  %65 = call i32 (ptr, ...) @printf(ptr @anon.string.35)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L28
  %66 = call %__String__charptr_long_long @_mf__String__String__getSubstring__int_long(ptr %s4, i32 2, i64 0)
  store %__String__charptr_long_long %66, ptr %6, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %sub, ptr %6, i64 24, i1 false)
  %67 = call ptr @_mf__String__string__getRaw(ptr %sub)
  %68 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %67, ptr @anon.string.36)
  br i1 %68, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L30
  %69 = call i32 (ptr, ...) @printf(ptr @anon.string.37)
  call void @exit(i32 1)
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L30
  %70 = call %__String__charptr_long_long @_mf__String__String__getSubstring__long_long(ptr %s4, i64 2, i64 12)
  store %__String__charptr_long_long %70, ptr %7, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %sub, ptr %7, i64 24, i1 false)
  %71 = call ptr @_mf__String__string__getRaw(ptr %sub)
  %72 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %71, ptr @anon.string.38)
  br i1 %72, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L32
  %73 = call i32 (ptr, ...) @printf(ptr @anon.string.39)
  call void @exit(i32 1)
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L32
  %74 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %75 = load i32, ptr %result, align 4
  ret i32 %75
}

declare void @_mp__String__void__ctor__string(ptr, ptr)

declare i1 @_mf__String__bool__isEmpty(ptr)

declare i32 @printf(ptr noundef, ...)

declare void @exit(i32)

declare i64 @_mf__String__long__getLength(ptr)

declare i64 @_mf__String__long__getCapacity(ptr)

declare i1 @_mf__String__bool__isFull(ptr)

declare i64 @_mf__String__long__find__string(ptr, ptr)

declare i64 @_mf__String__long__find__string_int(ptr, ptr, i32)

declare i1 @_mf__String__bool__contains__string(ptr, ptr)

declare %__String__charptr_long_long @_mf__String__String__getSubstring__int_long(ptr, i32, i64)

declare ptr @_mf__String__string__getRaw(ptr)

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr)

declare %__String__charptr_long_long @_mf__String__String__getSubstring__long_long(ptr, i64, i64)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare %__String__charptr_long_long @_mf__String__String__getSubstring__int(ptr, i32)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!0 = !{!"branch_weights", i32 2000, i32 1}
