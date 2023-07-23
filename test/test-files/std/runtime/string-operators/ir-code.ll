; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.String = type { ptr, i64, i64 }

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
@printf.str.11 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.11 = private unnamed_addr constant [4 x i8] c"One\00", align 1
@printf.str.12 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.12 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.13 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.13 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.14 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.15 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.14 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.16 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.17 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.15 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.18 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.19 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.16 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.20 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.21 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.17 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.22 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.23 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.18 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.24 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.25 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.19 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.26 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.27 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.20 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.28 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.29 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.21 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.30 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.31 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.22 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.32 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.33 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.23 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.34 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.35 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.36 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.24 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.37 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@anon.string.38 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@printf.str.25 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.39 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@printf.str.26 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %struct.String, align 8
  %2 = alloca %struct.String, align 8
  %3 = alloca %struct.String, align 8
  %4 = alloca %struct.String, align 8
  %5 = alloca %struct.String, align 8
  %s1 = alloca %struct.String, align 8
  %6 = alloca ptr, align 8
  %7 = alloca %struct.String, align 8
  %8 = alloca %struct.String, align 8
  %9 = alloca %struct.String, align 8
  %10 = alloca %struct.String, align 8
  %11 = alloca ptr, align 8
  %12 = alloca %struct.String, align 8
  %13 = alloca %struct.String, align 8
  %14 = alloca %struct.String, align 8
  %15 = alloca %struct.String, align 8
  %16 = alloca ptr, align 8
  %17 = alloca %struct.String, align 8
  %18 = alloca %struct.String, align 8
  %19 = alloca %struct.String, align 8
  %20 = alloca %struct.String, align 8
  %s2 = alloca %struct.String, align 8
  %21 = alloca %struct.String, align 8
  %22 = alloca %struct.String, align 8
  %23 = alloca %struct.String, align 8
  %24 = alloca %struct.String, align 8
  %s3 = alloca %struct.String, align 8
  %25 = alloca %struct.String, align 8
  %26 = alloca %struct.String, align 8
  %27 = alloca %struct.String, align 8
  %28 = alloca %struct.String, align 8
  %29 = alloca %struct.String, align 8
  %30 = alloca %struct.String, align 8
  %31 = alloca %struct.String, align 8
  %32 = alloca %struct.String, align 8
  %33 = alloca %struct.String, align 8
  %34 = alloca %struct.String, align 8
  %35 = alloca %struct.String, align 8
  %36 = alloca %struct.String, align 8
  %37 = alloca %struct.String, align 8
  %38 = alloca %struct.String, align 8
  %s4 = alloca %struct.String, align 8
  %39 = alloca i8, align 1
  %s5 = alloca %struct.String, align 8
  %40 = alloca ptr, align 8
  %s6 = alloca %struct.String, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN6String4ctorEPc(ptr %1, ptr @anon.string.0)
  call void @_ZN6String4ctorEPc(ptr %2, ptr @anon.string.1)
  %41 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %1, ptr %2)
  store %struct.String %41, ptr %3, align 8
  %42 = getelementptr inbounds %struct.String, ptr %3, i32 0, i32 0
  %43 = load ptr, ptr %42, align 8
  %44 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %43)
  call void @_ZN6String4ctorEPc(ptr %4, ptr @anon.string.2)
  call void @_ZN6String4ctorEPc(ptr %5, ptr @anon.string.3)
  %45 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %4, ptr %5)
  store %struct.String %45, ptr %s1, align 8
  store %struct.String %45, ptr %s1, align 8
  %46 = getelementptr inbounds %struct.String, ptr %s1, i32 0, i32 0
  %47 = load ptr, ptr %46, align 8
  %48 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %47)
  store ptr @anon.string.4, ptr %6, align 8
  %49 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr %s1, ptr %6)
  store %struct.String %49, ptr %7, align 8
  %50 = getelementptr inbounds %struct.String, ptr %7, i32 0, i32 0
  %51 = load ptr, ptr %50, align 8
  %52 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %51)
  call void @_ZN6String4ctorEPc(ptr %8, ptr @anon.string.5)
  %53 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %8, ptr %s1)
  store %struct.String %53, ptr %9, align 8
  %54 = getelementptr inbounds %struct.String, ptr %9, i32 0, i32 0
  %55 = load ptr, ptr %54, align 8
  %56 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, ptr %55)
  %57 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %s1, ptr %s1)
  store %struct.String %57, ptr %10, align 8
  %58 = getelementptr inbounds %struct.String, ptr %10, i32 0, i32 0
  %59 = load ptr, ptr %58, align 8
  %60 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, ptr %59)
  store ptr @anon.string.6, ptr %11, align 8
  %61 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr %s1, ptr %11)
  store %struct.String %61, ptr %12, align 8
  %62 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %12, ptr %s1)
  store %struct.String %62, ptr %13, align 8
  %63 = getelementptr inbounds %struct.String, ptr %13, i32 0, i32 0
  %64 = load ptr, ptr %63, align 8
  %65 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, ptr %64)
  call void @_ZN6String4ctorEPc(ptr %14, ptr @anon.string.7)
  %66 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %14, ptr %s1)
  store %struct.String %66, ptr %15, align 8
  store ptr @anon.string.8, ptr %16, align 8
  %67 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr %15, ptr %16)
  store %struct.String %67, ptr %17, align 8
  %68 = getelementptr inbounds %struct.String, ptr %17, i32 0, i32 0
  %69 = load ptr, ptr %68, align 8
  %70 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, ptr %69)
  call void @_ZN6String4ctorEPc(ptr %18, ptr @anon.string.9)
  %71 = call %struct.String @_Z6op.mulsRK6String(i16 4, ptr %18)
  store %struct.String %71, ptr %19, align 8
  %72 = getelementptr inbounds %struct.String, ptr %19, i32 0, i32 0
  %73 = load ptr, ptr %72, align 8
  %74 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, ptr %73)
  call void @_ZN6String4ctorEPc(ptr %20, ptr @anon.string.10)
  %75 = call %struct.String @_Z6op.mulRK6Stringi(ptr %20, i32 5)
  store %struct.String %75, ptr %s2, align 8
  store %struct.String %75, ptr %s2, align 8
  %76 = getelementptr inbounds %struct.String, ptr %s2, i32 0, i32 0
  %77 = load ptr, ptr %76, align 8
  %78 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, ptr %77)
  call void @_ZN6String4ctorEh(ptr %21, i8 97)
  %79 = call %struct.String @_Z6op.muliRK6String(i32 20, ptr %21)
  store %struct.String %79, ptr %22, align 8
  %80 = getelementptr inbounds %struct.String, ptr %22, i32 0, i32 0
  %81 = load ptr, ptr %80, align 8
  %82 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.9, ptr %81)
  call void @_ZN6String4ctorEh(ptr %23, i8 99)
  %83 = call %struct.String @_Z6op.muliRK6String(i32 2, ptr %23)
  store %struct.String %83, ptr %24, align 8
  %84 = call %struct.String @_Z6op.mulRK6Stringi(ptr %24, i32 7)
  store %struct.String %84, ptr %s3, align 8
  store %struct.String %84, ptr %s3, align 8
  %85 = getelementptr inbounds %struct.String, ptr %s3, i32 0, i32 0
  %86 = load ptr, ptr %85, align 8
  %87 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.10, ptr %86)
  call void @_ZN6String4ctorEPc(ptr %25, ptr @anon.string.11)
  %88 = call %struct.String @_Z6op.mulRK6Stringi(ptr %25, i32 1)
  store %struct.String %88, ptr %26, align 8
  %89 = getelementptr inbounds %struct.String, ptr %26, i32 0, i32 0
  %90 = load ptr, ptr %89, align 8
  %91 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.11, ptr %90)
  %92 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.12, ptr @anon.string.13)
  %93 = zext i1 %92 to i32
  %94 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.12, i32 %93)
  %95 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.14, ptr @anon.string.15)
  %96 = zext i1 %95 to i32
  %97 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.13, i32 %96)
  %98 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.16, ptr @anon.string.17)
  %99 = zext i1 %98 to i32
  %100 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.14, i32 %99)
  call void @_ZN6String4ctorEPc(ptr %27, ptr @anon.string.18)
  call void @_ZN6String4ctorEPc(ptr %28, ptr @anon.string.19)
  %101 = call i1 @_Z8op.equalRK6StringRK6String(ptr %27, ptr %28)
  %102 = zext i1 %101 to i32
  %103 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.15, i32 %102)
  call void @_ZN6String4ctorEPc(ptr %29, ptr @anon.string.20)
  call void @_ZN6String4ctorEPc(ptr %30, ptr @anon.string.21)
  %104 = call i1 @_Z8op.equalRK6StringRK6String(ptr %29, ptr %30)
  %105 = zext i1 %104 to i32
  %106 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.16, i32 %105)
  call void @_ZN6String4ctorEPc(ptr %31, ptr @anon.string.22)
  call void @_ZN6String4ctorEPc(ptr %32, ptr @anon.string.23)
  %107 = call i1 @_Z8op.equalRK6StringRK6String(ptr %31, ptr %32)
  %108 = zext i1 %107 to i32
  %109 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.17, i32 %108)
  %110 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.24, ptr @anon.string.25)
  %111 = xor i1 %110, true
  %112 = zext i1 %111 to i32
  %113 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.18, i32 %112)
  %114 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.26, ptr @anon.string.27)
  %115 = xor i1 %114, true
  %116 = zext i1 %115 to i32
  %117 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.19, i32 %116)
  %118 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.28, ptr @anon.string.29)
  %119 = xor i1 %118, true
  %120 = zext i1 %119 to i32
  %121 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.20, i32 %120)
  call void @_ZN6String4ctorEPc(ptr %33, ptr @anon.string.30)
  call void @_ZN6String4ctorEPc(ptr %34, ptr @anon.string.31)
  %122 = call i1 @_Z11op.notequalRK6StringRK6String(ptr %33, ptr %34)
  %123 = zext i1 %122 to i32
  %124 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.21, i32 %123)
  call void @_ZN6String4ctorEPc(ptr %35, ptr @anon.string.32)
  call void @_ZN6String4ctorEPc(ptr %36, ptr @anon.string.33)
  %125 = call i1 @_Z11op.notequalRK6StringRK6String(ptr %35, ptr %36)
  %126 = zext i1 %125 to i32
  %127 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.22, i32 %126)
  call void @_ZN6String4ctorEPc(ptr %37, ptr @anon.string.34)
  call void @_ZN6String4ctorEPc(ptr %38, ptr @anon.string.35)
  %128 = call i1 @_Z11op.notequalRK6StringRK6String(ptr %37, ptr %38)
  %129 = zext i1 %128 to i32
  %130 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.23, i32 %129)
  call void @_ZN6String4ctorEPc(ptr %s4, ptr @anon.string.36)
  store i8 108, ptr %39, align 1
  call void @_Z12op.plusequalR6StringRKh(ptr %s4, ptr %39)
  %131 = getelementptr inbounds %struct.String, ptr %s4, i32 0, i32 0
  %132 = load ptr, ptr %131, align 8
  %133 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.24, ptr %132)
  call void @_ZN6String4ctorEPc(ptr %s5, ptr @anon.string.37)
  store ptr @anon.string.38, ptr %40, align 8
  call void @_Z12op.plusequalR6StringRKPc(ptr %s5, ptr %40)
  %134 = getelementptr inbounds %struct.String, ptr %s5, i32 0, i32 0
  %135 = load ptr, ptr %134, align 8
  %136 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.25, ptr %135)
  call void @_ZN6String4ctorEPc(ptr %s6, ptr @anon.string.39)
  call void @_Z11op.mulequalR6Stringi(ptr %s6, i32 3)
  %137 = getelementptr inbounds %struct.String, ptr %s6, i32 0, i32 0
  %138 = load ptr, ptr %137, align 8
  %139 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.26, ptr %138)
  call void @_ZN6String4dtorEv(ptr %37)
  call void @_ZN6String4dtorEv(ptr %38)
  call void @_ZN6String4dtorEv(ptr %36)
  call void @_ZN6String4dtorEv(ptr %33)
  call void @_ZN6String4dtorEv(ptr %34)
  call void @_ZN6String4dtorEv(ptr %31)
  call void @_ZN6String4dtorEv(ptr %10)
  call void @_ZN6String4dtorEv(ptr %15)
  call void @_ZN6String4dtorEv(ptr %24)
  call void @_ZN6String4dtorEv(ptr %12)
  call void @_ZN6String4dtorEv(ptr %7)
  call void @_ZN6String4dtorEv(ptr %5)
  call void @_ZN6String4dtorEv(ptr %35)
  call void @_ZN6String4dtorEv(ptr %2)
  call void @_ZN6String4dtorEv(ptr %s6)
  call void @_ZN6String4dtorEv(ptr %s4)
  call void @_ZN6String4dtorEv(ptr %17)
  call void @_ZN6String4dtorEv(ptr %s5)
  call void @_ZN6String4dtorEv(ptr %3)
  call void @_ZN6String4dtorEv(ptr %s3)
  call void @_ZN6String4dtorEv(ptr %s1)
  call void @_ZN6String4dtorEv(ptr %13)
  call void @_ZN6String4dtorEv(ptr %s2)
  call void @_ZN6String4dtorEv(ptr %18)
  call void @_ZN6String4dtorEv(ptr %23)
  call void @_ZN6String4dtorEv(ptr %21)
  call void @_ZN6String4dtorEv(ptr %28)
  call void @_ZN6String4dtorEv(ptr %22)
  call void @_ZN6String4dtorEv(ptr %19)
  call void @_ZN6String4dtorEv(ptr %26)
  call void @_ZN6String4dtorEv(ptr %27)
  call void @_ZN6String4dtorEv(ptr %9)
  call void @_ZN6String4dtorEv(ptr %30)
  call void @_ZN6String4dtorEv(ptr %29)
  call void @_ZN6String4dtorEv(ptr %32)
  %140 = load i32, ptr %result, align 4
  ret i32 %140
}

declare i32 @printf(ptr noundef, ...)

declare void @_ZN6String4ctorEPc(ptr, ptr)

declare %struct.String @_Z7op.plusRK6StringRK6String(ptr, ptr)

declare %struct.String @_Z7op.plusRK6StringRKPc(ptr, ptr)

declare %struct.String @_Z6op.mulsRK6String(i16, ptr)

declare %struct.String @_Z6op.mulRK6Stringi(ptr, i32)

declare void @_ZN6String4ctorEh(ptr, i8)

declare %struct.String @_Z6op.muliRK6String(i32, ptr)

declare i1 @_Z10isRawEqualPcPc(ptr, ptr)

declare i1 @_Z8op.equalRK6StringRK6String(ptr, ptr)

declare i1 @_Z11op.notequalRK6StringRK6String(ptr, ptr)

declare void @_Z12op.plusequalR6StringRKh(ptr, ptr)

declare void @_Z12op.plusequalR6StringRKPc(ptr, ptr)

declare void @_Z11op.mulequalR6Stringi(ptr, i32)

declare void @_ZN6String4dtorEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
