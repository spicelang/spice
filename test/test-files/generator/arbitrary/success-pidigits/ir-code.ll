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
  %q_new = alloca i64, align 8
  %r = alloca i64, align 8
  %r_new = alloca i64, align 8
  %t = alloca i64, align 8
  %t_new = alloca i64, align 8
  %k = alloca i64, align 8
  %k_new = alloca i64, align 8
  %m = alloca i64, align 8
  %m_new = alloca i64, align 8
  %x = alloca i64, align 8
  %x_new = alloca i64, align 8
  %iterations = alloca i32, align 4
  %printedDigits = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca i64, align 8
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  %4 = alloca i1, align 1
  %5 = alloca i1, align 1
  %6 = alloca i32, align 4
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i64, align 8
  %13 = alloca i64, align 8
  %14 = alloca i64, align 8
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
  %30 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i64 1, i64* %q, align 8
  store i64 0, i64* %q_new, align 8
  store i64 0, i64* %r, align 8
  store i64 0, i64* %r_new, align 8
  store i64 1, i64* %t, align 8
  store i64 0, i64* %t_new, align 8
  store i64 1, i64* %k, align 8
  store i64 0, i64* %k_new, align 8
  store i64 3, i64* %m, align 8
  store i64 0, i64* %m_new, align 8
  store i64 3, i64* %x, align 8
  store i64 0, i64* %x_new, align 8
  store i32 20, i32* %iterations, align 4
  store i32 0, i32* %printedDigits, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post, %entry
  %31 = load i32, i32* %i, align 4
  %32 = load i32, i32* %iterations, align 4
  %33 = icmp slt i32 %31, %32
  store i1 %33, i1* %0, align 1
  %34 = load i1, i1* %0, align 1
  br i1 %34, label %for, label %for.end

for:                                              ; preds = %for.cond
  %35 = load i64, i64* %q, align 8
  %36 = mul i64 4, %35
  store i64 %36, i64* %1, align 8
  %37 = load i64, i64* %1, align 8
  %38 = load i64, i64* %r, align 8
  %39 = add i64 %37, %38
  %40 = load i64, i64* %t, align 8
  %41 = sub i64 %39, %40
  store i64 %41, i64* %2, align 8
  %42 = load i64, i64* %2, align 8
  %43 = load i64, i64* %m, align 8
  %44 = load i64, i64* %t, align 8
  %45 = mul i64 %43, %44
  store i64 %45, i64* %3, align 8
  %46 = load i64, i64* %3, align 8
  %47 = icmp slt i64 %42, %46
  store i1 %47, i1* %4, align 1
  %48 = load i1, i1* %4, align 1
  br i1 %48, label %if.then, label %if.else

if.then:                                          ; preds = %for
  %49 = load i64, i64* %m, align 8
  %50 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i64 %49)
  %51 = load i32, i32* %printedDigits, align 4
  %52 = icmp eq i32 %51, 0
  store i1 %52, i1* %5, align 1
  %53 = load i1, i1* %5, align 1
  br i1 %53, label %if.then1, label %if.end

if.then1:                                         ; preds = %if.then
  %54 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @1, i32 0, i32 0))
  br label %if.end

if.end:                                           ; preds = %if.then1, %if.then
  %55 = load i32, i32* %printedDigits, align 4
  %56 = add i32 %55, 1
  store i32 %56, i32* %printedDigits, align 4
  store i32 %55, i32* %6, align 4
  %57 = load i64, i64* %q, align 8
  %58 = mul i64 10, %57
  store i64 %58, i64* %7, align 8
  %59 = load i64, i64* %7, align 8
  store i64 %59, i64* %q_new, align 8
  %60 = load i64, i64* %r, align 8
  %61 = load i64, i64* %m, align 8
  %62 = load i64, i64* %t, align 8
  %63 = mul i64 %61, %62
  store i64 %63, i64* %8, align 8
  %64 = load i64, i64* %8, align 8
  %65 = sub i64 %60, %64
  store i64 %65, i64* %9, align 8
  %66 = load i64, i64* %9, align 8
  %67 = mul i64 10, %66
  store i64 %67, i64* %10, align 8
  %68 = load i64, i64* %10, align 8
  store i64 %68, i64* %r_new, align 8
  %69 = load i64, i64* %q, align 8
  %70 = mul i64 3, %69
  store i64 %70, i64* %11, align 8
  %71 = load i64, i64* %11, align 8
  %72 = load i64, i64* %r, align 8
  %73 = add i64 %71, %72
  store i64 %73, i64* %12, align 8
  %74 = load i64, i64* %12, align 8
  %75 = mul i64 10, %74
  store i64 %75, i64* %13, align 8
  %76 = load i64, i64* %13, align 8
  %77 = load i64, i64* %t, align 8
  %78 = sdiv i64 %76, %77
  store i64 %78, i64* %14, align 8
  %79 = load i64, i64* %14, align 8
  %80 = load i64, i64* %m, align 8
  %81 = mul i64 10, %80
  store i64 %81, i64* %15, align 8
  %82 = load i64, i64* %15, align 8
  %83 = sub i64 %79, %82
  store i64 %83, i64* %16, align 8
  %84 = load i64, i64* %16, align 8
  store i64 %84, i64* %m, align 8
  %85 = load i64, i64* %q_new, align 8
  store i64 %85, i64* %q, align 8
  %86 = load i64, i64* %r_new, align 8
  store i64 %86, i64* %r, align 8
  br label %if.end2

if.else:                                          ; preds = %for
  %87 = load i64, i64* %q, align 8
  %88 = load i64, i64* %k, align 8
  %89 = mul i64 %87, %88
  store i64 %89, i64* %17, align 8
  %90 = load i64, i64* %17, align 8
  store i64 %90, i64* %q_new, align 8
  %91 = load i64, i64* %q, align 8
  %92 = mul i64 2, %91
  store i64 %92, i64* %18, align 8
  %93 = load i64, i64* %18, align 8
  %94 = load i64, i64* %r, align 8
  %95 = add i64 %93, %94
  store i64 %95, i64* %19, align 8
  %96 = load i64, i64* %19, align 8
  %97 = load i64, i64* %x, align 8
  %98 = mul i64 %96, %97
  store i64 %98, i64* %20, align 8
  %99 = load i64, i64* %20, align 8
  store i64 %99, i64* %r_new, align 8
  %100 = load i64, i64* %t, align 8
  %101 = load i64, i64* %x, align 8
  %102 = mul i64 %100, %101
  store i64 %102, i64* %21, align 8
  %103 = load i64, i64* %21, align 8
  store i64 %103, i64* %t_new, align 8
  %104 = load i64, i64* %k, align 8
  %105 = add i64 %104, 1
  store i64 %105, i64* %22, align 8
  %106 = load i64, i64* %22, align 8
  store i64 %106, i64* %k_new, align 8
  %107 = load i64, i64* %q, align 8
  %108 = load i64, i64* %k, align 8
  %109 = mul i64 7, %108
  store i64 %109, i64* %23, align 8
  %110 = load i64, i64* %23, align 8
  %111 = add i64 %110, 2
  store i64 %111, i64* %24, align 8
  %112 = load i64, i64* %24, align 8
  %113 = mul i64 %107, %112
  store i64 %113, i64* %25, align 8
  %114 = load i64, i64* %25, align 8
  %115 = load i64, i64* %r, align 8
  %116 = load i64, i64* %x, align 8
  %117 = mul i64 %115, %116
  store i64 %117, i64* %26, align 8
  %118 = load i64, i64* %26, align 8
  %119 = add i64 %114, %118
  store i64 %119, i64* %27, align 8
  %120 = load i64, i64* %27, align 8
  %121 = load i64, i64* %t, align 8
  %122 = load i64, i64* %x, align 8
  %123 = mul i64 %121, %122
  store i64 %123, i64* %28, align 8
  %124 = load i64, i64* %28, align 8
  %125 = sdiv i64 %120, %124
  store i64 %125, i64* %29, align 8
  %126 = load i64, i64* %29, align 8
  store i64 %126, i64* %m, align 8
  %127 = load i64, i64* %x, align 8
  %128 = add i64 %127, 2
  store i64 %128, i64* %x, align 8
  %129 = load i64, i64* %q_new, align 8
  store i64 %129, i64* %q, align 8
  %130 = load i64, i64* %r_new, align 8
  store i64 %130, i64* %r, align 8
  %131 = load i64, i64* %t_new, align 8
  store i64 %131, i64* %t, align 8
  %132 = load i64, i64* %k_new, align 8
  store i64 %132, i64* %k, align 8
  br label %if.end2

if.end2:                                          ; preds = %if.else, %if.end
  br label %for.post

for.post:                                         ; preds = %if.end2
  %133 = load i32, i32* %i, align 4
  %134 = add i32 %133, 1
  store i32 %134, i32* %i, align 4
  store i32 %133, i32* %30, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %135 = load i32, i32* %result, align 4
  ret i32 %135
}

declare i32 @printf(i8*, ...)
