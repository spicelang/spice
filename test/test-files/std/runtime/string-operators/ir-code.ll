; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.3 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.4 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@printf.str.3 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.5 = private unnamed_addr constant [5 x i8] c"Hi! \00", align 1
@printf.str.4 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@printf.str.6 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.7 = private unnamed_addr constant [8 x i8] c"Prefix \00", align 1
@anon.string.8 = private unnamed_addr constant [8 x i8] c" Suffix\00", align 1
@printf.str.7 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.9 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@anon.string.10 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@printf.str.8 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.9 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.10 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.11 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.11 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.12 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.12 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.13 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.14 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.13 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.15 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.16 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.14 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.17 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.18 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.15 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.19 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.20 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.16 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.21 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.22 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.17 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.23 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.24 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.18 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.25 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.26 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.19 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.27 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.28 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.20 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.29 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.30 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.21 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.31 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.32 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.22 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.33 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.34 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %s1 = alloca %__String__charptr_long_long, align 8
  %3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %5 = alloca %__String__charptr_long_long, align 8
  %6 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %7 = alloca %__String__charptr_long_long, align 8
  %8 = alloca %__String__charptr_long_long, align 8
  %s3 = alloca %__String__charptr_long_long, align 8
  %9 = alloca %__String__charptr_long_long, align 8
  %10 = alloca %__String__charptr_long_long, align 8
  %11 = alloca %__String__charptr_long_long, align 8
  %12 = alloca %__String__charptr_long_long, align 8
  %13 = alloca %__String__charptr_long_long, align 8
  %14 = alloca %__String__charptr_long_long, align 8
  %15 = alloca %__String__charptr_long_long, align 8
  %16 = alloca %__String__charptr_long_long, align 8
  %17 = alloca %__String__charptr_long_long, align 8
  %18 = alloca %__String__charptr_long_long, align 8
  %19 = alloca %__String__charptr_long_long, align 8
  %20 = alloca %__String__charptr_long_long, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__String__void__ctor__string(ptr %1, ptr @anon.string.0)
  call void @_mp__String__void__ctor__string(ptr %2, ptr @anon.string.1)
  %21 = load %__String__charptr_long_long, ptr %1, align 8
  %22 = load %__String__charptr_long_long, ptr %2, align 8
  %23 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %21, %__String__charptr_long_long %22)
  %24 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, %__String__charptr_long_long %23)
  call void @_mp__String__void__ctor__string(ptr %s1, ptr @anon.string.2)
  call void @_mp__String__void__ctor__string(ptr %3, ptr @anon.string.3)
  %25 = load %__String__charptr_long_long, ptr %s1, align 8
  %26 = load %__String__charptr_long_long, ptr %3, align 8
  %27 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %25, %__String__charptr_long_long %26)
  store %__String__charptr_long_long %27, ptr %s1, align 8
  %28 = load %__String__charptr_long_long, ptr %s1, align 8
  %29 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, %__String__charptr_long_long %28)
  %30 = load %__String__charptr_long_long, ptr %s1, align 8
  %31 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %30, ptr @anon.string.4)
  %32 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, %__String__charptr_long_long %31)
  call void @_mp__String__void__ctor__string(ptr %4, ptr @anon.string.5)
  %33 = load %__String__charptr_long_long, ptr %4, align 8
  %34 = load %__String__charptr_long_long, ptr %s1, align 8
  %35 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %33, %__String__charptr_long_long %34)
  %36 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, %__String__charptr_long_long %35)
  %37 = load %__String__charptr_long_long, ptr %s1, align 8
  %38 = load %__String__charptr_long_long, ptr %s1, align 8
  %39 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %37, %__String__charptr_long_long %38)
  %40 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, %__String__charptr_long_long %39)
  %41 = load %__String__charptr_long_long, ptr %s1, align 8
  %42 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %41, ptr @anon.string.6)
  %43 = load %__String__charptr_long_long, ptr %s1, align 8
  %44 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %42, %__String__charptr_long_long %43)
  %45 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, %__String__charptr_long_long %44)
  call void @_mp__String__void__ctor__string(ptr %5, ptr @anon.string.7)
  %46 = load %__String__charptr_long_long, ptr %5, align 8
  %47 = load %__String__charptr_long_long, ptr %s1, align 8
  %48 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %46, %__String__charptr_long_long %47)
  %49 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %48, ptr @anon.string.8)
  %50 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, %__String__charptr_long_long %49)
  call void @_mp__String__void__ctor__string(ptr %6, ptr @anon.string.9)
  %51 = load %__String__charptr_long_long, ptr %6, align 8
  %52 = call %__String__charptr_long_long @_f__void__String__op.mul__short_String(i16 4, %__String__charptr_long_long %51)
  %53 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, %__String__charptr_long_long %52)
  call void @_mp__String__void__ctor__string(ptr %s2, ptr @anon.string.10)
  %54 = load %__String__charptr_long_long, ptr %s2, align 8
  %55 = call %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long %54, i32 5)
  store %__String__charptr_long_long %55, ptr %s2, align 8
  %56 = load %__String__charptr_long_long, ptr %s2, align 8
  %57 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, %__String__charptr_long_long %56)
  call void @_mp__String__void__ctor__char(ptr %7, i8 97)
  %58 = load %__String__charptr_long_long, ptr %7, align 8
  %59 = call %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32 20, %__String__charptr_long_long %58)
  %60 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.9, %__String__charptr_long_long %59)
  call void @_mp__String__void__ctor__char(ptr %8, i8 99)
  %61 = load %__String__charptr_long_long, ptr %8, align 8
  %62 = call %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32 2, %__String__charptr_long_long %61)
  %63 = call %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long %62, i32 7)
  store %__String__charptr_long_long %63, ptr %s3, align 8
  store %__String__charptr_long_long %63, ptr %s3, align 8
  %64 = load %__String__charptr_long_long, ptr %s3, align 8
  %65 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.10, %__String__charptr_long_long %64)
  %66 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.11, ptr @anon.string.12)
  %67 = zext i1 %66 to i32
  %68 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.11, i32 %67)
  %69 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.13, ptr @anon.string.14)
  %70 = zext i1 %69 to i32
  %71 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.12, i32 %70)
  %72 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.15, ptr @anon.string.16)
  %73 = zext i1 %72 to i32
  %74 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.13, i32 %73)
  call void @_mp__String__void__ctor__string(ptr %9, ptr @anon.string.17)
  call void @_mp__String__void__ctor__string(ptr %10, ptr @anon.string.18)
  %75 = load %__String__charptr_long_long, ptr %9, align 8
  %76 = load %__String__charptr_long_long, ptr %10, align 8
  %77 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %75, %__String__charptr_long_long %76)
  %78 = zext i1 %77 to i32
  %79 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.14, i32 %78)
  call void @_mp__String__void__ctor__string(ptr %11, ptr @anon.string.19)
  call void @_mp__String__void__ctor__string(ptr %12, ptr @anon.string.20)
  %80 = load %__String__charptr_long_long, ptr %11, align 8
  %81 = load %__String__charptr_long_long, ptr %12, align 8
  %82 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %80, %__String__charptr_long_long %81)
  %83 = zext i1 %82 to i32
  %84 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.15, i32 %83)
  call void @_mp__String__void__ctor__string(ptr %13, ptr @anon.string.21)
  call void @_mp__String__void__ctor__string(ptr %14, ptr @anon.string.22)
  %85 = load %__String__charptr_long_long, ptr %13, align 8
  %86 = load %__String__charptr_long_long, ptr %14, align 8
  %87 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %85, %__String__charptr_long_long %86)
  %88 = zext i1 %87 to i32
  %89 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.16, i32 %88)
  %90 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.23, ptr @anon.string.24)
  %91 = xor i1 %90, true
  %92 = zext i1 %91 to i32
  %93 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.17, i32 %92)
  %94 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.25, ptr @anon.string.26)
  %95 = xor i1 %94, true
  %96 = zext i1 %95 to i32
  %97 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.18, i32 %96)
  %98 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.27, ptr @anon.string.28)
  %99 = xor i1 %98, true
  %100 = zext i1 %99 to i32
  %101 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.19, i32 %100)
  call void @_mp__String__void__ctor__string(ptr %15, ptr @anon.string.29)
  call void @_mp__String__void__ctor__string(ptr %16, ptr @anon.string.30)
  %102 = load %__String__charptr_long_long, ptr %15, align 8
  %103 = load %__String__charptr_long_long, ptr %16, align 8
  %104 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %102, %__String__charptr_long_long %103)
  %105 = zext i1 %104 to i32
  %106 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.20, i32 %105)
  call void @_mp__String__void__ctor__string(ptr %17, ptr @anon.string.31)
  call void @_mp__String__void__ctor__string(ptr %18, ptr @anon.string.32)
  %107 = load %__String__charptr_long_long, ptr %17, align 8
  %108 = load %__String__charptr_long_long, ptr %18, align 8
  %109 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %107, %__String__charptr_long_long %108)
  %110 = zext i1 %109 to i32
  %111 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.21, i32 %110)
  call void @_mp__String__void__ctor__string(ptr %19, ptr @anon.string.33)
  call void @_mp__String__void__ctor__string(ptr %20, ptr @anon.string.34)
  %112 = load %__String__charptr_long_long, ptr %19, align 8
  %113 = load %__String__charptr_long_long, ptr %20, align 8
  %114 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %112, %__String__charptr_long_long %113)
  %115 = zext i1 %114 to i32
  %116 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.22, i32 %115)
  %117 = load i32, ptr %result, align 4
  ret i32 %117
}

declare i32 @printf(ptr noundef, ...)

declare void @_mp__String__void__ctor__string(ptr, ptr)

declare %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long, %__String__charptr_long_long)

declare %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long, ptr)

declare %__String__charptr_long_long @_f__void__String__op.mul__short_String(i16, %__String__charptr_long_long)

declare %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long, i32)

declare void @_mp__String__void__ctor__char(ptr, i8)

declare %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32, %__String__charptr_long_long)

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr)

declare i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long, %__String__charptr_long_long)

declare i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long, %__String__charptr_long_long)

attributes #0 = { noinline nounwind optnone uwtable }
