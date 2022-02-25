; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [2 x i8] c".\00", align 1

declare i32 @printf(i8*, ...)

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
  %9 = alloca i1, align 1
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i64, align 8
  %13 = alloca i64, align 8
  %14 = alloca i1, align 1
  %15 = alloca i32, align 4
  %16 = alloca i1, align 1
  %17 = alloca i32, align 4
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
  %38 = alloca i64, align 8
  %39 = alloca i64, align 8
  %40 = alloca i64, align 8
  %41 = alloca i64, align 8
  %42 = alloca i64, align 8
  %43 = alloca i64, align 8
  %44 = alloca i64, align 8
  %45 = alloca i64, align 8
  %46 = alloca i64, align 8
  %47 = alloca i64, align 8
  %48 = alloca i64, align 8
  %49 = alloca i64, align 8
  %50 = alloca i64, align 8
  %51 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i64 1, i64* %0, align 8
  %52 = load i64, i64* %0, align 8
  store i64 %52, i64* %q, align 8
  store i64 0, i64* %1, align 8
  %53 = load i64, i64* %1, align 8
  store i64 %53, i64* %r, align 8
  store i64 1, i64* %2, align 8
  %54 = load i64, i64* %2, align 8
  store i64 %54, i64* %t, align 8
  store i64 1, i64* %3, align 8
  %55 = load i64, i64* %3, align 8
  store i64 %55, i64* %k, align 8
  store i64 3, i64* %4, align 8
  %56 = load i64, i64* %4, align 8
  store i64 %56, i64* %m, align 8
  store i64 3, i64* %5, align 8
  %57 = load i64, i64* %5, align 8
  store i64 %57, i64* %x, align 8
  store i32 20, i32* %6, align 4
  %58 = load i32, i32* %6, align 4
  store i32 %58, i32* %iterations, align 4
  store i32 0, i32* %7, align 4
  %59 = load i32, i32* %7, align 4
  store i32 %59, i32* %printedDigits, align 4
  store i32 0, i32* %8, align 4
  %60 = load i32, i32* %8, align 4
  store i32 %60, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post, %entry
  %61 = load i32, i32* %i, align 4
  %62 = load i32, i32* %iterations, align 4
  %63 = icmp slt i32 %61, %62
  store i1 %63, i1* %9, align 1
  %64 = load i1, i1* %9, align 1
  br i1 %64, label %for, label %for.end

for:                                              ; preds = %for.cond
  store i64 4, i64* %10, align 8
  %65 = load i64, i64* %10, align 8
  %66 = load i64, i64* %q, align 8
  %67 = mul i64 %65, %66
  store i64 %67, i64* %11, align 8
  %68 = load i64, i64* %11, align 8
  %69 = load i64, i64* %r, align 8
  %70 = add i64 %68, %69
  %71 = load i64, i64* %t, align 8
  %72 = sub i64 %70, %71
  store i64 %72, i64* %12, align 8
  %73 = load i64, i64* %m, align 8
  %74 = load i64, i64* %t, align 8
  %75 = mul i64 %73, %74
  store i64 %75, i64* %13, align 8
  %76 = load i64, i64* %12, align 8
  %77 = load i64, i64* %13, align 8
  %78 = icmp slt i64 %76, %77
  store i1 %78, i1* %14, align 1
  %79 = load i1, i1* %14, align 1
  br i1 %79, label %if.then, label %if.else

if.then:                                          ; preds = %for
  %80 = load i64, i64* %m, align 8
  %81 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i64 %80)
  store i32 0, i32* %15, align 4
  %82 = load i32, i32* %printedDigits, align 4
  %83 = load i32, i32* %15, align 4
  %84 = icmp eq i32 %82, %83
  store i1 %84, i1* %16, align 1
  %85 = load i1, i1* %16, align 1
  br i1 %85, label %if.then1, label %if.end

if.then1:                                         ; preds = %if.then
  %86 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @1, i32 0, i32 0))
  br label %if.end

if.end:                                           ; preds = %if.then1, %if.then
  %87 = load i32, i32* %printedDigits, align 4
  %88 = add i32 %87, 1
  store i32 %88, i32* %printedDigits, align 4
  store i32 %87, i32* %17, align 4
  store i64 10, i64* %18, align 8
  %89 = load i64, i64* %18, align 8
  %90 = load i64, i64* %q, align 8
  %91 = mul i64 %89, %90
  store i64 %91, i64* %19, align 8
  %92 = load i64, i64* %19, align 8
  store i64 %92, i64* %q_new, align 8
  store i64 10, i64* %20, align 8
  %93 = load i64, i64* %20, align 8
  %94 = load i64, i64* %r, align 8
  %95 = load i64, i64* %m, align 8
  %96 = load i64, i64* %t, align 8
  %97 = mul i64 %95, %96
  store i64 %97, i64* %21, align 8
  %98 = load i64, i64* %21, align 8
  %99 = sub i64 %94, %98
  store i64 %99, i64* %22, align 8
  %100 = load i64, i64* %22, align 8
  %101 = mul i64 %93, %100
  store i64 %101, i64* %23, align 8
  %102 = load i64, i64* %23, align 8
  store i64 %102, i64* %r_new, align 8
  store i64 10, i64* %24, align 8
  %103 = load i64, i64* %24, align 8
  store i64 3, i64* %25, align 8
  %104 = load i64, i64* %25, align 8
  %105 = load i64, i64* %q, align 8
  %106 = mul i64 %104, %105
  store i64 %106, i64* %26, align 8
  %107 = load i64, i64* %26, align 8
  %108 = load i64, i64* %r, align 8
  %109 = add i64 %107, %108
  store i64 %109, i64* %27, align 8
  %110 = load i64, i64* %27, align 8
  %111 = mul i64 %103, %110
  store i64 %111, i64* %28, align 8
  %112 = load i64, i64* %28, align 8
  %113 = load i64, i64* %t, align 8
  %114 = sdiv i64 %112, %113
  store i64 %114, i64* %29, align 8
  %115 = load i64, i64* %29, align 8
  store i64 10, i64* %30, align 8
  %116 = load i64, i64* %30, align 8
  %117 = load i64, i64* %m, align 8
  %118 = mul i64 %116, %117
  store i64 %118, i64* %31, align 8
  %119 = load i64, i64* %31, align 8
  %120 = sub i64 %115, %119
  store i64 %120, i64* %32, align 8
  %121 = load i64, i64* %32, align 8
  store i64 %121, i64* %m, align 8
  %122 = load i64, i64* %q_new, align 8
  store i64 %122, i64* %q, align 8
  %123 = load i64, i64* %r_new, align 8
  store i64 %123, i64* %r, align 8
  br label %if.end2

if.else:                                          ; preds = %for
  %124 = load i64, i64* %q, align 8
  %125 = load i64, i64* %k, align 8
  %126 = mul i64 %124, %125
  store i64 %126, i64* %33, align 8
  %127 = load i64, i64* %33, align 8
  store i64 %127, i64* %q_new, align 8
  store i64 2, i64* %34, align 8
  %128 = load i64, i64* %34, align 8
  %129 = load i64, i64* %q, align 8
  %130 = mul i64 %128, %129
  store i64 %130, i64* %35, align 8
  %131 = load i64, i64* %35, align 8
  %132 = load i64, i64* %r, align 8
  %133 = add i64 %131, %132
  store i64 %133, i64* %36, align 8
  %134 = load i64, i64* %36, align 8
  %135 = load i64, i64* %x, align 8
  %136 = mul i64 %134, %135
  store i64 %136, i64* %37, align 8
  %137 = load i64, i64* %37, align 8
  store i64 %137, i64* %r_new, align 8
  %138 = load i64, i64* %t, align 8
  %139 = load i64, i64* %x, align 8
  %140 = mul i64 %138, %139
  store i64 %140, i64* %38, align 8
  %141 = load i64, i64* %38, align 8
  store i64 %141, i64* %t_new, align 8
  %142 = load i64, i64* %k, align 8
  store i64 1, i64* %39, align 8
  %143 = load i64, i64* %39, align 8
  %144 = add i64 %142, %143
  store i64 %144, i64* %40, align 8
  %145 = load i64, i64* %40, align 8
  store i64 %145, i64* %k_new, align 8
  %146 = load i64, i64* %q, align 8
  store i64 7, i64* %41, align 8
  %147 = load i64, i64* %41, align 8
  %148 = load i64, i64* %k, align 8
  %149 = mul i64 %147, %148
  store i64 %149, i64* %42, align 8
  %150 = load i64, i64* %42, align 8
  store i64 2, i64* %43, align 8
  %151 = load i64, i64* %43, align 8
  %152 = add i64 %150, %151
  store i64 %152, i64* %44, align 8
  %153 = load i64, i64* %44, align 8
  %154 = mul i64 %146, %153
  store i64 %154, i64* %45, align 8
  %155 = load i64, i64* %45, align 8
  %156 = load i64, i64* %r, align 8
  %157 = load i64, i64* %x, align 8
  %158 = mul i64 %156, %157
  store i64 %158, i64* %46, align 8
  %159 = load i64, i64* %46, align 8
  %160 = add i64 %155, %159
  store i64 %160, i64* %47, align 8
  %161 = load i64, i64* %47, align 8
  %162 = load i64, i64* %t, align 8
  %163 = load i64, i64* %x, align 8
  %164 = mul i64 %162, %163
  store i64 %164, i64* %48, align 8
  %165 = load i64, i64* %48, align 8
  %166 = sdiv i64 %161, %165
  store i64 %166, i64* %49, align 8
  %167 = load i64, i64* %49, align 8
  store i64 %167, i64* %m, align 8
  store i64 2, i64* %50, align 8
  %168 = load i64, i64* %50, align 8
  %169 = load i64, i64* %x, align 8
  %170 = add i64 %169, %168
  store i64 %170, i64* %x, align 8
  %171 = load i64, i64* %q_new, align 8
  store i64 %171, i64* %q, align 8
  %172 = load i64, i64* %r_new, align 8
  store i64 %172, i64* %r, align 8
  %173 = load i64, i64* %t_new, align 8
  store i64 %173, i64* %t, align 8
  %174 = load i64, i64* %k_new, align 8
  store i64 %174, i64* %k, align 8
  br label %if.end2

if.end2:                                          ; preds = %if.else, %if.end
  br label %for.post

for.post:                                         ; preds = %if.end2
  %175 = load i32, i32* %i, align 4
  %176 = add i32 %175, 1
  store i32 %176, i32* %i, align 4
  store i32 %175, i32* %51, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %177 = load i32, i32* %result, align 4
  ret i32 %177
}
