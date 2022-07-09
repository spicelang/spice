; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [2 x i8] c".\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %q = alloca i64, align 8
  %0 = alloca i64, align 8
  %q_new = alloca i64, align 8
  %r = alloca i64, align 8
  %1 = alloca i64, align 8
  %r_new = alloca i64, align 8
  %t = alloca i64, align 8
  %2 = alloca i64, align 8
  %t_new = alloca i64, align 8
  %k = alloca i64, align 8
  %3 = alloca i64, align 8
  %k_new = alloca i64, align 8
  %m = alloca i64, align 8
  %4 = alloca i64, align 8
  %m_new = alloca i64, align 8
  %x = alloca i64, align 8
  %5 = alloca i64, align 8
  %x_new = alloca i64, align 8
  %iterations = alloca i32, align 4
  %6 = alloca i32, align 4
  %printedDigits = alloca i32, align 4
  %7 = alloca i32, align 4
  %i = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i1, align 1
  %13 = alloca i1, align 1
  %14 = alloca i32, align 4
  %15 = alloca i64, align 8
  %16 = alloca i64, align 8
  %17 = alloca i64, align 8
  %18 = alloca i64, align 8
  %19 = alloca i64, align 8
  %20 = alloca i64, align 8
  %21 = alloca i64, align 8
  %22 = alloca i64, align 8
  %23 = alloca i64, align 8
  %24 = alloca i64, align 8
  %25 = alloca i64, align 8
  %26 = alloca i64, align 8
  %27 = alloca i64, align 8
  %28 = alloca i64, align 8
  %29 = alloca i64, align 8
  %30 = alloca i64, align 8
  %31 = alloca i64, align 8
  %32 = alloca i64, align 8
  %33 = alloca i64, align 8
  %34 = alloca i64, align 8
  %35 = alloca i64, align 8
  %36 = alloca i64, align 8
  %37 = alloca i64, align 8
  %38 = alloca i32, align 4
  %39 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i64 1, i64* %0, align 8
  store i64 0, i64* %q_new, align 8
  store i64 0, i64* %1, align 8
  store i64 0, i64* %r_new, align 8
  store i64 1, i64* %2, align 8
  store i64 0, i64* %t_new, align 8
  store i64 1, i64* %3, align 8
  store i64 0, i64* %k_new, align 8
  store i64 3, i64* %4, align 8
  store i64 0, i64* %m_new, align 8
  store i64 3, i64* %5, align 8
  store i64 0, i64* %x_new, align 8
  store i32 20, i32* %6, align 4
  store i32 0, i32* %7, align 4
  store i32 0, i32* %8, align 4
  br label %for.cond

for:                                              ; preds = %for.cond
  %40 = load i64, i64* %0, align 8
  %41 = mul i64 4, %40
  store i64 %41, i64* %9, align 8
  %42 = load i64, i64* %9, align 8
  %43 = load i64, i64* %1, align 8
  %44 = add i64 %42, %43
  %45 = load i64, i64* %2, align 8
  %46 = sub i64 %44, %45
  store i64 %46, i64* %10, align 8
  %47 = load i64, i64* %10, align 8
  %48 = load i64, i64* %4, align 8
  %49 = load i64, i64* %2, align 8
  %50 = mul i64 %48, %49
  store i64 %50, i64* %11, align 8
  %51 = load i64, i64* %11, align 8
  %52 = icmp slt i64 %47, %51
  store i1 %52, i1* %12, align 1
  %53 = load i1, i1* %12, align 1
  br i1 %53, label %if.then, label %if.else

if.then:                                          ; preds = %for
  %54 = load i64, i64* %4, align 8
  %55 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i64 %54)
  %56 = load i32, i32* %7, align 4
  %57 = icmp eq i32 %56, 0
  store i1 %57, i1* %13, align 1
  %58 = load i1, i1* %13, align 1
  br i1 %58, label %if.then1, label %if.end

if.then1:                                         ; preds = %if.then
  %59 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @1, i32 0, i32 0))
  br label %if.end

if.end:                                           ; preds = %if.then1, %if.then
  %60 = load i32, i32* %7, align 4
  %61 = add i32 %60, 1
  store i32 %61, i32* %7, align 4
  store i32 %60, i32* %14, align 4
  %62 = load i64, i64* %0, align 8
  %63 = mul i64 10, %62
  store i64 %63, i64* %15, align 8
  %64 = load i64, i64* %15, align 8
  store i64 %64, i64* %q_new, align 8
  %65 = load i64, i64* %1, align 8
  %66 = load i64, i64* %4, align 8
  %67 = load i64, i64* %2, align 8
  %68 = mul i64 %66, %67
  store i64 %68, i64* %16, align 8
  %69 = load i64, i64* %16, align 8
  %70 = sub i64 %65, %69
  store i64 %70, i64* %17, align 8
  %71 = load i64, i64* %17, align 8
  %72 = mul i64 10, %71
  store i64 %72, i64* %18, align 8
  %73 = load i64, i64* %18, align 8
  store i64 %73, i64* %r_new, align 8
  %74 = load i64, i64* %0, align 8
  %75 = mul i64 3, %74
  store i64 %75, i64* %19, align 8
  %76 = load i64, i64* %19, align 8
  %77 = load i64, i64* %1, align 8
  %78 = add i64 %76, %77
  store i64 %78, i64* %20, align 8
  %79 = load i64, i64* %20, align 8
  %80 = mul i64 10, %79
  store i64 %80, i64* %21, align 8
  %81 = load i64, i64* %21, align 8
  %82 = load i64, i64* %2, align 8
  %83 = sdiv i64 %81, %82
  store i64 %83, i64* %22, align 8
  %84 = load i64, i64* %22, align 8
  %85 = load i64, i64* %4, align 8
  %86 = mul i64 10, %85
  store i64 %86, i64* %23, align 8
  %87 = load i64, i64* %23, align 8
  %88 = sub i64 %84, %87
  store i64 %88, i64* %24, align 8
  %89 = load i64, i64* %24, align 8
  store i64 %89, i64* %4, align 8
  %90 = load i64, i64* %q_new, align 8
  store i64 %90, i64* %0, align 8
  %91 = load i64, i64* %r_new, align 8
  store i64 %91, i64* %1, align 8
  br label %if.end2

if.else:                                          ; preds = %for
  %92 = load i64, i64* %0, align 8
  %93 = load i64, i64* %3, align 8
  %94 = mul i64 %92, %93
  store i64 %94, i64* %25, align 8
  %95 = load i64, i64* %25, align 8
  store i64 %95, i64* %q_new, align 8
  %96 = load i64, i64* %0, align 8
  %97 = mul i64 2, %96
  store i64 %97, i64* %26, align 8
  %98 = load i64, i64* %26, align 8
  %99 = load i64, i64* %1, align 8
  %100 = add i64 %98, %99
  store i64 %100, i64* %27, align 8
  %101 = load i64, i64* %27, align 8
  %102 = load i64, i64* %5, align 8
  %103 = mul i64 %101, %102
  store i64 %103, i64* %28, align 8
  %104 = load i64, i64* %28, align 8
  store i64 %104, i64* %r_new, align 8
  %105 = load i64, i64* %2, align 8
  %106 = load i64, i64* %5, align 8
  %107 = mul i64 %105, %106
  store i64 %107, i64* %29, align 8
  %108 = load i64, i64* %29, align 8
  store i64 %108, i64* %t_new, align 8
  %109 = load i64, i64* %3, align 8
  %110 = add i64 %109, 1
  store i64 %110, i64* %30, align 8
  %111 = load i64, i64* %30, align 8
  store i64 %111, i64* %k_new, align 8
  %112 = load i64, i64* %0, align 8
  %113 = load i64, i64* %3, align 8
  %114 = mul i64 7, %113
  store i64 %114, i64* %31, align 8
  %115 = load i64, i64* %31, align 8
  %116 = add i64 %115, 2
  store i64 %116, i64* %32, align 8
  %117 = load i64, i64* %32, align 8
  %118 = mul i64 %112, %117
  store i64 %118, i64* %33, align 8
  %119 = load i64, i64* %33, align 8
  %120 = load i64, i64* %1, align 8
  %121 = load i64, i64* %5, align 8
  %122 = mul i64 %120, %121
  store i64 %122, i64* %34, align 8
  %123 = load i64, i64* %34, align 8
  %124 = add i64 %119, %123
  store i64 %124, i64* %35, align 8
  %125 = load i64, i64* %35, align 8
  %126 = load i64, i64* %2, align 8
  %127 = load i64, i64* %5, align 8
  %128 = mul i64 %126, %127
  store i64 %128, i64* %36, align 8
  %129 = load i64, i64* %36, align 8
  %130 = sdiv i64 %125, %129
  store i64 %130, i64* %37, align 8
  %131 = load i64, i64* %37, align 8
  store i64 %131, i64* %4, align 8
  %132 = load i64, i64* %5, align 8
  %133 = add i64 %132, 2
  store i64 %133, i64* %5, align 8
  %134 = load i64, i64* %q_new, align 8
  store i64 %134, i64* %0, align 8
  %135 = load i64, i64* %r_new, align 8
  store i64 %135, i64* %1, align 8
  %136 = load i64, i64* %t_new, align 8
  store i64 %136, i64* %2, align 8
  %137 = load i64, i64* %k_new, align 8
  store i64 %137, i64* %3, align 8
  br label %if.end2

if.end2:                                          ; preds = %if.else, %if.end
  br label %for.inc

for.inc:                                          ; preds = %if.end2
  %138 = load i32, i32* %8, align 4
  %139 = add i32 %138, 1
  store i32 %139, i32* %8, align 4
  store i32 %138, i32* %38, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %140 = load i32, i32* %8, align 4
  %141 = load i32, i32* %6, align 4
  %142 = icmp slt i32 %140, %141
  store i1 %142, i1* %39, align 1
  %143 = load i1, i1* %39, align 1
  br i1 %143, label %for, label %for.end

for.end:                                          ; preds = %for.cond
  %144 = load i32, i32* %result, align 4
  ret i32 %144
}

declare i32 @printf(i8*, ...)
