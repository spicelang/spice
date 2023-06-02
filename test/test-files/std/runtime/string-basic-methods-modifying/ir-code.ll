; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.2 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getRaw() == \22Hello\22' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's.getLength() == 6' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getCapacity() == 12' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.6 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.7 = private unnamed_addr constant [79 x i8] c"Assertion failed: Condition 's.getRaw() == \22Hello World!\22' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's.getLength() == 12' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getCapacity() == 12' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [14 x i8] c"Hello World!?\00", align 1
@anon.string.11 = private unnamed_addr constant [80 x i8] c"Assertion failed: Condition 's.getRaw() == \22Hello World!?\22' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's.getLength() == 13' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getCapacity() == 24' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [15 x i8] c"Hello World!?!\00", align 1
@anon.string.15 = private unnamed_addr constant [81 x i8] c"Assertion failed: Condition 's.getRaw() == \22Hello World!?!\22' evaluated to false.\00", align 1
@anon.string.16 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's.getLength() == 14' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getCapacity() == 24' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.19 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 's.getRaw() == \22\22' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's.getLength() == 0' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getCapacity() == 24' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.23 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 's.getRaw() == \22\22' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's.getLength() == 0' evaluated to false.\00", align 1
@anon.string.25 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's.getCapacity() == 100' evaluated to false.\00", align 1
@anon.string.26 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.27 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 's.isEmpty()' evaluated to false.\00", align 1
@anon.string.28 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition '!s.isEmpty()' evaluated to false.\00", align 1
@anon.string.29 = private unnamed_addr constant [58 x i8] c"This is a test. And because this is a test, it is a test.\00", align 1
@anon.string.30 = private unnamed_addr constant [4 x i8] c"foo\00", align 1
@anon.string.31 = private unnamed_addr constant [5 x i8] c"demo\00", align 1
@anon.string.32 = private unnamed_addr constant [76 x i8] c"Assertion failed: Condition '!s.replace(\22foo\22, \22demo\22)' evaluated to false.\00", align 1
@anon.string.33 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@anon.string.34 = private unnamed_addr constant [5 x i8] c"demo\00", align 1
@anon.string.35 = private unnamed_addr constant [76 x i8] c"Assertion failed: Condition 's.replace(\22test\22, \22demo\22)' evaluated to false.\00", align 1
@anon.string.36 = private unnamed_addr constant [58 x i8] c"This is a demo. And because this is a test, it is a test.\00", align 1
@anon.string.37 = private unnamed_addr constant [124 x i8] c"Assertion failed: Condition 's.getRaw() == \22This is a demo. And because this is a test, it is a test.\22' evaluated to false.\00", align 1
@anon.string.38 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@anon.string.39 = private unnamed_addr constant [14 x i8] c"demonstration\00", align 1
@anon.string.40 = private unnamed_addr constant [90 x i8] c"Assertion failed: Condition 's.replace(\22test\22, \22demonstration\22, 30l)' evaluated to false.\00", align 1
@anon.string.41 = private unnamed_addr constant [67 x i8] c"This is a demo. And because this is a test, it is a demonstration.\00", align 1
@anon.string.42 = private unnamed_addr constant [133 x i8] c"Assertion failed: Condition 's.getRaw() == \22This is a demo. And because this is a test, it is a demonstration.\22' evaluated to false.\00", align 1
@anon.string.43 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@anon.string.44 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@anon.string.45 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's.replace(\22test\22, \22d\22)' evaluated to false.\00", align 1
@anon.string.46 = private unnamed_addr constant [64 x i8] c"This is a demo. And because this is a d, it is a demonstration.\00", align 1
@anon.string.47 = private unnamed_addr constant [130 x i8] c"Assertion failed: Condition 's.getRaw() == \22This is a demo. And because this is a d, it is a demonstration.\22' evaluated to false.\00", align 1
@anon.string.48 = private unnamed_addr constant [3 x i8] c"is\00", align 1
@anon.string.49 = private unnamed_addr constant [4 x i8] c"was\00", align 1
@anon.string.50 = private unnamed_addr constant [81 x i8] c"Assertion failed: Condition 's.replaceAll(\22is\22, \22was\22) == 3' evaluated to false.\00", align 1
@anon.string.51 = private unnamed_addr constant [61 x i8] c"This was a demo. And because this was a demo, it was a demo.\00", align 1
@anon.string.52 = private unnamed_addr constant [127 x i8] c"Assertion failed: Condition 's.getRaw() == \22This was a demo. And because this was a demo, it was a demo.\22' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %s = alloca %__String__charptr_long_long, align 8
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca i1, align 1
  %3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  call void @_mp__String__void__ctor__string(ptr %s, ptr @anon.string.0)
  %5 = call ptr @_mf__String__string__getRaw(ptr %s)
  %6 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %5, ptr @anon.string.1)
  br i1 %6, label %assert.exit.L3, label %assert.then.L3, !prof !0

assert.then.L3:                                   ; preds = %0
  %7 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L3:                                   ; preds = %0
  %8 = call i64 @_mf__String__long__getLength(ptr %s)
  %9 = icmp eq i64 %8, 6
  br i1 %9, label %assert.exit.L4, label %assert.then.L4, !prof !0

assert.then.L4:                                   ; preds = %assert.exit.L3
  %10 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L4:                                   ; preds = %assert.exit.L3
  %11 = call i64 @_mf__String__long__getCapacity(ptr %s)
  %12 = icmp eq i64 %11, 12
  br i1 %12, label %assert.exit.L5, label %assert.then.L5, !prof !0

assert.then.L5:                                   ; preds = %assert.exit.L4
  %13 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L5:                                   ; preds = %assert.exit.L4
  call void @_mp__String__void__append__string(ptr %s, ptr @anon.string.5)
  %14 = call ptr @_mf__String__string__getRaw(ptr %s)
  %15 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %14, ptr @anon.string.6)
  br i1 %15, label %assert.exit.L7, label %assert.then.L7, !prof !0

assert.then.L7:                                   ; preds = %assert.exit.L5
  %16 = call i32 (ptr, ...) @printf(ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L7:                                   ; preds = %assert.exit.L5
  %17 = call i64 @_mf__String__long__getLength(ptr %s)
  %18 = icmp eq i64 %17, 12
  br i1 %18, label %assert.exit.L8, label %assert.then.L8, !prof !0

assert.then.L8:                                   ; preds = %assert.exit.L7
  %19 = call i32 (ptr, ...) @printf(ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L8:                                   ; preds = %assert.exit.L7
  %20 = call i64 @_mf__String__long__getCapacity(ptr %s)
  %21 = icmp eq i64 %20, 12
  br i1 %21, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %assert.exit.L8
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %assert.exit.L8
  call void @_mp__String__void__append__char(ptr %s, i8 63)
  %23 = call ptr @_mf__String__string__getRaw(ptr %s)
  %24 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %23, ptr @anon.string.10)
  br i1 %24, label %assert.exit.L11, label %assert.then.L11, !prof !0

assert.then.L11:                                  ; preds = %assert.exit.L9
  %25 = call i32 (ptr, ...) @printf(ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L9
  %26 = call i64 @_mf__String__long__getLength(ptr %s)
  %27 = icmp eq i64 %26, 13
  br i1 %27, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %assert.exit.L11
  %28 = call i32 (ptr, ...) @printf(ptr @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L11
  %29 = call i64 @_mf__String__long__getCapacity(ptr %s)
  %30 = icmp eq i64 %29, 24
  br i1 %30, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L12
  %31 = call i32 (ptr, ...) @printf(ptr @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L12
  call void @_mp__String__void__append__char(ptr %s, i8 33)
  %32 = call ptr @_mf__String__string__getRaw(ptr %s)
  %33 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %32, ptr @anon.string.14)
  br i1 %33, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L13
  %34 = call i32 (ptr, ...) @printf(ptr @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L13
  %35 = call i64 @_mf__String__long__getLength(ptr %s)
  %36 = icmp eq i64 %35, 14
  br i1 %36, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %assert.exit.L15
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.16)
  call void @exit(i32 1)
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L15
  %38 = call i64 @_mf__String__long__getCapacity(ptr %s)
  %39 = icmp eq i64 %38, 24
  br i1 %39, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L16
  %40 = call i32 (ptr, ...) @printf(ptr @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L16
  call void @_mp__String__void__clear(ptr %s)
  %41 = call ptr @_mf__String__string__getRaw(ptr %s)
  %42 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %41, ptr @anon.string.18)
  br i1 %42, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %assert.exit.L17
  %43 = call i32 (ptr, ...) @printf(ptr @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L17
  %44 = call i64 @_mf__String__long__getLength(ptr %s)
  %45 = icmp eq i64 %44, 0
  br i1 %45, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L19
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.20)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L19
  %47 = call i64 @_mf__String__long__getCapacity(ptr %s)
  %48 = icmp eq i64 %47, 24
  br i1 %48, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L20
  %49 = call i32 (ptr, ...) @printf(ptr @anon.string.21)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  call void @_mp__String__void__reserve__long(ptr %s, i64 100)
  %50 = call ptr @_mf__String__string__getRaw(ptr %s)
  %51 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %50, ptr @anon.string.22)
  br i1 %51, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L21
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.23)
  call void @exit(i32 1)
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L21
  %53 = call i64 @_mf__String__long__getLength(ptr %s)
  %54 = icmp eq i64 %53, 0
  br i1 %54, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L23
  %55 = call i32 (ptr, ...) @printf(ptr @anon.string.24)
  call void @exit(i32 1)
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L23
  %56 = call i64 @_mf__String__long__getCapacity(ptr %s)
  %57 = icmp eq i64 %56, 100
  br i1 %57, label %assert.exit.L25, label %assert.then.L25, !prof !0

assert.then.L25:                                  ; preds = %assert.exit.L24
  %58 = call i32 (ptr, ...) @printf(ptr @anon.string.25)
  call void @exit(i32 1)
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L24
  call void @_mp__String__void__ctor__string(ptr %1, ptr @anon.string.26)
  call void @llvm.memcpy.p0.p0.i64(ptr %s, ptr %1, i64 24, i1 false)
  %59 = call i1 @_mf__String__bool__isEmpty(ptr %s)
  br i1 %59, label %assert.exit.L27, label %assert.then.L27, !prof !0

assert.then.L27:                                  ; preds = %assert.exit.L25
  %60 = call i32 (ptr, ...) @printf(ptr @anon.string.27)
  call void @exit(i32 1)
  unreachable

assert.exit.L27:                                  ; preds = %assert.exit.L25
  call void @_mp__String__void__append__char(ptr %s, i8 97)
  %61 = call i1 @_mf__String__bool__isEmpty(ptr %s)
  %62 = xor i1 %61, true
  store i1 %62, ptr %2, align 1
  br i1 %62, label %assert.exit.L29, label %assert.then.L29, !prof !0

assert.then.L29:                                  ; preds = %assert.exit.L27
  %63 = call i32 (ptr, ...) @printf(ptr @anon.string.28)
  call void @exit(i32 1)
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L27
  call void @_mp__String__void__ctor__string(ptr %3, ptr @anon.string.29)
  call void @llvm.memcpy.p0.p0.i64(ptr %s, ptr %3, i64 24, i1 false)
  %64 = call i1 @_mf__String__bool__replace__string_string(ptr %s, ptr @anon.string.30, ptr @anon.string.31)
  %65 = xor i1 %64, true
  store i1 %65, ptr %4, align 1
  br i1 %65, label %assert.exit.L31, label %assert.then.L31, !prof !0

assert.then.L31:                                  ; preds = %assert.exit.L29
  %66 = call i32 (ptr, ...) @printf(ptr @anon.string.32)
  call void @exit(i32 1)
  unreachable

assert.exit.L31:                                  ; preds = %assert.exit.L29
  %67 = call i1 @_mf__String__bool__replace__string_string(ptr %s, ptr @anon.string.33, ptr @anon.string.34)
  br i1 %67, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L31
  %68 = call i32 (ptr, ...) @printf(ptr @anon.string.35)
  call void @exit(i32 1)
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L31
  %69 = call ptr @_mf__String__string__getRaw(ptr %s)
  %70 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %69, ptr @anon.string.36)
  br i1 %70, label %assert.exit.L33, label %assert.then.L33, !prof !0

assert.then.L33:                                  ; preds = %assert.exit.L32
  %71 = call i32 (ptr, ...) @printf(ptr @anon.string.37)
  call void @exit(i32 1)
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L32
  %72 = call i1 @_mf__String__bool__replace__string_string_long(ptr %s, ptr @anon.string.38, ptr @anon.string.39, i64 30)
  br i1 %72, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L33
  %73 = call i32 (ptr, ...) @printf(ptr @anon.string.40)
  call void @exit(i32 1)
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %74 = call ptr @_mf__String__string__getRaw(ptr %s)
  %75 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %74, ptr @anon.string.41)
  br i1 %75, label %assert.exit.L35, label %assert.then.L35, !prof !0

assert.then.L35:                                  ; preds = %assert.exit.L34
  %76 = call i32 (ptr, ...) @printf(ptr @anon.string.42)
  call void @exit(i32 1)
  unreachable

assert.exit.L35:                                  ; preds = %assert.exit.L34
  %77 = call i1 @_mf__String__bool__replace__string_string(ptr %s, ptr @anon.string.43, ptr @anon.string.44)
  br i1 %77, label %assert.exit.L36, label %assert.then.L36, !prof !0

assert.then.L36:                                  ; preds = %assert.exit.L35
  %78 = call i32 (ptr, ...) @printf(ptr @anon.string.45)
  call void @exit(i32 1)
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L35
  %79 = call ptr @_mf__String__string__getRaw(ptr %s)
  %80 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %79, ptr @anon.string.46)
  br i1 %80, label %assert.exit.L37, label %assert.then.L37, !prof !0

assert.then.L37:                                  ; preds = %assert.exit.L36
  %81 = call i32 (ptr, ...) @printf(ptr @anon.string.47)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %82 = call i64 @_mf__String__long__replaceAll__string_string(ptr %s, ptr @anon.string.48, ptr @anon.string.49)
  %83 = icmp eq i64 %82, 3
  br i1 %83, label %assert.exit.L38, label %assert.then.L38, !prof !0

assert.then.L38:                                  ; preds = %assert.exit.L37
  %84 = call i32 (ptr, ...) @printf(ptr @anon.string.50)
  call void @exit(i32 1)
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L37
  %85 = call ptr @_mf__String__string__getRaw(ptr %s)
  %86 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %85, ptr @anon.string.51)
  br i1 %86, label %assert.exit.L39, label %assert.then.L39, !prof !0

assert.then.L39:                                  ; preds = %assert.exit.L38
  %87 = call i32 (ptr, ...) @printf(ptr @anon.string.52)
  call void @exit(i32 1)
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L38
  %88 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %89 = load i32, ptr %result, align 4
  ret i32 %89
}

declare void @_mp__String__void__ctor__string(ptr, ptr)

declare ptr @_mf__String__string__getRaw(ptr)

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr)

declare i32 @printf(ptr noundef, ...)

declare void @exit(i32)

declare i64 @_mf__String__long__getLength(ptr)

declare i64 @_mf__String__long__getCapacity(ptr)

declare void @_mp__String__void__append__string(ptr, ptr)

declare void @_mp__String__void__append__char(ptr, i8)

declare void @_mp__String__void__clear(ptr)

declare void @_mp__String__void__reserve__long(ptr, i64)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i1 @_mf__String__bool__isEmpty(ptr)

declare i1 @_mf__String__bool__replace__string_string(ptr, ptr, ptr)

declare i1 @_mf__String__bool__replace__string_string_long(ptr, ptr, ptr, i64)

declare i64 @_mf__String__long__replaceAll__string_string(ptr, ptr, ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!0 = !{!"branch_weights", i32 2000, i32 1}
