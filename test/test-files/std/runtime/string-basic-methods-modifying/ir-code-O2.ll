; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.2 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getRaw() == \22Hello\22' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's.getLength() == 6' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.6 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.7 = private unnamed_addr constant [79 x i8] c"Assertion failed: Condition 's.getRaw() == \22Hello World!\22' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's.getLength() == 12' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getCapacity() == 12' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [14 x i8] c"Hello World!?\00", align 1
@anon.string.11 = private unnamed_addr constant [80 x i8] c"Assertion failed: Condition 's.getRaw() == \22Hello World!?\22' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's.getLength() == 13' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [15 x i8] c"Hello World!?!\00", align 1
@anon.string.15 = private unnamed_addr constant [81 x i8] c"Assertion failed: Condition 's.getRaw() == \22Hello World!?!\22' evaluated to false.\00", align 1
@anon.string.16 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's.getLength() == 14' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's.getCapacity() == 24' evaluated to false.\00", align 1
@anon.string.23 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 's.getRaw() == \22\22' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's.getLength() == 0' evaluated to false.\00", align 1
@anon.string.25 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's.getCapacity() == 100' evaluated to false.\00", align 1
@anon.string.26 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.27 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 's.isEmpty()' evaluated to false.\00", align 1
@anon.string.28 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition '!s.isEmpty()' evaluated to false.\00", align 1
@anon.string.29 = private unnamed_addr constant [58 x i8] c"This is a test. And because this is a test, it is a test.\00", align 1
@anon.string.30 = private unnamed_addr constant [4 x i8] c"foo\00", align 1
@anon.string.32 = private unnamed_addr constant [76 x i8] c"Assertion failed: Condition '!s.replace(\22foo\22, \22demo\22)' evaluated to false.\00", align 1
@anon.string.34 = private unnamed_addr constant [5 x i8] c"demo\00", align 1
@anon.string.35 = private unnamed_addr constant [76 x i8] c"Assertion failed: Condition 's.replace(\22test\22, \22demo\22)' evaluated to false.\00", align 1
@anon.string.36 = private unnamed_addr constant [58 x i8] c"This is a demo. And because this is a test, it is a test.\00", align 1
@anon.string.37 = private unnamed_addr constant [124 x i8] c"Assertion failed: Condition 's.getRaw() == \22This is a demo. And because this is a test, it is a test.\22' evaluated to false.\00", align 1
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
define dso_local i32 @main() local_unnamed_addr #0 {
  %s = alloca %__String__charptr_long_long, align 8
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %s, ptr nonnull @anon.string.0) #3
  %3 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %4 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %3, ptr nonnull @anon.string.1) #3
  br i1 %4, label %assert.exit.L3, label %assert.then.L3, !prof !0

assert.then.L3:                                   ; preds = %0
  %5 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L3:                                   ; preds = %0
  %6 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %7 = icmp eq i64 %6, 6
  br i1 %7, label %assert.exit.L4, label %assert.then.L4, !prof !0

assert.then.L4:                                   ; preds = %assert.exit.L3
  %8 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.3)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L4:                                   ; preds = %assert.exit.L3
  %9 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %10 = icmp eq i64 %9, 12
  br i1 %10, label %assert.exit.L5, label %assert.then.L5, !prof !0

assert.then.L5:                                   ; preds = %assert.exit.L4
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L5:                                   ; preds = %assert.exit.L4
  call void @_mp__String__void__append__string(ptr nonnull %s, ptr nonnull @anon.string.5) #3
  %12 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %13 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %12, ptr nonnull @anon.string.6) #3
  br i1 %13, label %assert.exit.L7, label %assert.then.L7, !prof !0

assert.then.L7:                                   ; preds = %assert.exit.L5
  %14 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L7:                                   ; preds = %assert.exit.L5
  %15 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %16 = icmp eq i64 %15, 12
  br i1 %16, label %assert.exit.L8, label %assert.then.L8, !prof !0

assert.then.L8:                                   ; preds = %assert.exit.L7
  %17 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L8:                                   ; preds = %assert.exit.L7
  %18 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %19 = icmp eq i64 %18, 12
  br i1 %19, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %assert.exit.L8
  %20 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L9:                                   ; preds = %assert.exit.L8
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 63) #3
  %21 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %22 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %21, ptr nonnull @anon.string.10) #3
  br i1 %22, label %assert.exit.L11, label %assert.then.L11, !prof !0

assert.then.L11:                                  ; preds = %assert.exit.L9
  %23 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L9
  %24 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %25 = icmp eq i64 %24, 13
  br i1 %25, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %assert.exit.L11
  %26 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L11
  %27 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %28 = icmp eq i64 %27, 24
  br i1 %28, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L12
  %29 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L12
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 33) #3
  %30 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %31 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %30, ptr nonnull @anon.string.14) #3
  br i1 %31, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L13
  %32 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L13
  %33 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %34 = icmp eq i64 %33, 14
  br i1 %34, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %assert.exit.L15
  %35 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.16)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L15
  %36 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %37 = icmp eq i64 %36, 24
  br i1 %37, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L16
  %38 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L16
  call void @_mp__String__void__clear(ptr nonnull %s) #3
  %39 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %40 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %39, ptr nonnull @anon.string.26) #3
  br i1 %40, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %assert.exit.L17
  %41 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.23)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L17
  %42 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %43 = icmp eq i64 %42, 0
  br i1 %43, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L19
  %44 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.24)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L19
  %45 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %46 = icmp eq i64 %45, 24
  br i1 %46, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L20
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  call void @_mp__String__void__reserve__long(ptr nonnull %s, i64 100) #3
  %48 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %49 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %48, ptr nonnull @anon.string.26) #3
  br i1 %49, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L21
  %50 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.23)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L21
  %51 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %52 = icmp eq i64 %51, 0
  br i1 %52, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L23
  %53 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.24)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L23
  %54 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %55 = icmp eq i64 %54, 100
  br i1 %55, label %assert.exit.L25, label %assert.then.L25, !prof !0

assert.then.L25:                                  ; preds = %assert.exit.L24
  %56 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.25)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L24
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.26) #3
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(24) %s, ptr noundef nonnull align 8 dereferenceable(24) %1, i64 24, i1 false)
  %57 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s) #3
  br i1 %57, label %assert.exit.L27, label %assert.then.L27, !prof !0

assert.then.L27:                                  ; preds = %assert.exit.L25
  %58 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.27)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L27:                                  ; preds = %assert.exit.L25
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 97) #3
  %59 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s) #3
  br i1 %59, label %assert.then.L29, label %assert.exit.L29, !prof !1

assert.then.L29:                                  ; preds = %assert.exit.L27
  %60 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.28)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L27
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @anon.string.29) #3
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(24) %s, ptr noundef nonnull align 8 dereferenceable(24) %2, i64 24, i1 false)
  %61 = call i1 @_mf__String__bool__replace__string_string(ptr nonnull %s, ptr nonnull @anon.string.30, ptr nonnull @anon.string.34) #3
  br i1 %61, label %assert.then.L31, label %assert.exit.L31, !prof !1

assert.then.L31:                                  ; preds = %assert.exit.L29
  %62 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.32)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L31:                                  ; preds = %assert.exit.L29
  %63 = call i1 @_mf__String__bool__replace__string_string(ptr nonnull %s, ptr nonnull @anon.string.43, ptr nonnull @anon.string.34) #3
  br i1 %63, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L31
  %64 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.35)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L31
  %65 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %66 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %65, ptr nonnull @anon.string.36) #3
  br i1 %66, label %assert.exit.L33, label %assert.then.L33, !prof !0

assert.then.L33:                                  ; preds = %assert.exit.L32
  %67 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.37)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L32
  %68 = call i1 @_mf__String__bool__replace__string_string_long(ptr nonnull %s, ptr nonnull @anon.string.43, ptr nonnull @anon.string.39, i64 30) #3
  br i1 %68, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L33
  %69 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.40)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %70 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %71 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %70, ptr nonnull @anon.string.41) #3
  br i1 %71, label %assert.exit.L35, label %assert.then.L35, !prof !0

assert.then.L35:                                  ; preds = %assert.exit.L34
  %72 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.42)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L35:                                  ; preds = %assert.exit.L34
  %73 = call i1 @_mf__String__bool__replace__string_string(ptr nonnull %s, ptr nonnull @anon.string.43, ptr nonnull @anon.string.44) #3
  br i1 %73, label %assert.exit.L36, label %assert.then.L36, !prof !0

assert.then.L36:                                  ; preds = %assert.exit.L35
  %74 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.45)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L35
  %75 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %76 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %75, ptr nonnull @anon.string.46) #3
  br i1 %76, label %assert.exit.L37, label %assert.then.L37, !prof !0

assert.then.L37:                                  ; preds = %assert.exit.L36
  %77 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.47)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %78 = call i64 @_mf__String__long__replaceAll__string_string(ptr nonnull %s, ptr nonnull @anon.string.48, ptr nonnull @anon.string.49) #3
  %79 = icmp eq i64 %78, 3
  br i1 %79, label %assert.exit.L38, label %assert.then.L38, !prof !0

assert.then.L38:                                  ; preds = %assert.exit.L37
  %80 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.50)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L37
  %81 = call ptr @_mf__String__string__getRaw(ptr nonnull %s) #3
  %82 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %81, ptr nonnull @anon.string.51) #3
  br i1 %82, label %assert.exit.L39, label %assert.then.L39, !prof !0

assert.then.L39:                                  ; preds = %assert.exit.L38
  %83 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.52)
  call void @exit(i32 1) #3
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L38
  %84 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

declare ptr @_mf__String__string__getRaw(ptr) local_unnamed_addr

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

declare i64 @_mf__String__long__getLength(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getCapacity(ptr) local_unnamed_addr

declare void @_mp__String__void__append__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__void__append__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__void__clear(ptr) local_unnamed_addr

declare void @_mp__String__void__reserve__long(ptr, i64) local_unnamed_addr

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #2

declare i1 @_mf__String__bool__isEmpty(ptr) local_unnamed_addr

declare i1 @_mf__String__bool__replace__string_string(ptr, ptr, ptr) local_unnamed_addr

declare i1 @_mf__String__bool__replace__string_string_long(ptr, ptr, ptr, i64) local_unnamed_addr

declare i64 @_mf__String__long__replaceAll__string_string(ptr, ptr, ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
