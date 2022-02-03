; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define i32 @"makePi()"() #0 {
entry:
  %result = alloca i32, align 4
  %q = alloca i32, align 4
  %0 = alloca i32, align 4
  %r = alloca i32, align 4
  %1 = alloca i32, align 4
  %t = alloca i32, align 4
  %2 = alloca i32, align 4
  %k = alloca i32, align 4
  %3 = alloca i32, align 4
  %m = alloca i32, align 4
  %4 = alloca i32, align 4
  %x = alloca i32, align 4
  %5 = alloca i32, align 4
  %outputCounter = alloca i32, align 4
  %6 = alloca i32, align 4
  %i = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i1, align 1
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i1, align 1
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  %18 = alloca i32, align 4
  %19 = alloca i32, align 4
  %20 = alloca i32, align 4
  %21 = alloca i32, align 4
  %22 = alloca i32, align 4
  %23 = alloca i32, align 4
  %24 = alloca i32, align 4
  %25 = alloca i32, align 4
  %26 = alloca i32, align 4
  %27 = alloca i32, align 4
  %28 = alloca i32, align 4
  %29 = alloca i32, align 4
  %30 = alloca i32, align 4
  %31 = alloca i32, align 4
  %32 = alloca i32, align 4
  %33 = alloca i32, align 4
  %34 = alloca i32, align 4
  %35 = alloca i32, align 4
  %36 = alloca i32, align 4
  %37 = alloca i32, align 4
  %38 = alloca i32, align 4
  %39 = alloca i32, align 4
  %40 = alloca i32, align 4
  %41 = alloca i32, align 4
  %42 = alloca i32, align 4
  %43 = alloca i32, align 4
  %44 = alloca i32, align 4
  %45 = alloca i32, align 4
  %46 = alloca i32, align 4
  %47 = alloca i32, align 4
  %48 = alloca i32, align 4
  %49 = alloca i32, align 4
  store i32 1, i32* %0, align 4
  %50 = load i32, i32* %0, align 4
  store i32 %50, i32* %q, align 4
  store i32 0, i32* %1, align 4
  %51 = load i32, i32* %1, align 4
  store i32 %51, i32* %r, align 4
  store i32 1, i32* %2, align 4
  %52 = load i32, i32* %2, align 4
  store i32 %52, i32* %t, align 4
  store i32 1, i32* %3, align 4
  %53 = load i32, i32* %3, align 4
  store i32 %53, i32* %k, align 4
  store i32 3, i32* %4, align 4
  %54 = load i32, i32* %4, align 4
  store i32 %54, i32* %m, align 4
  store i32 3, i32* %5, align 4
  %55 = load i32, i32* %5, align 4
  store i32 %55, i32* %x, align 4
  store i32 0, i32* %6, align 4
  %56 = load i32, i32* %6, align 4
  store i32 %56, i32* %outputCounter, align 4
  store i32 0, i32* %7, align 4
  %57 = load i32, i32* %7, align 4
  store i32 %57, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %if.end, %entry
  store i32 1000, i32* %8, align 4
  %58 = load i32, i32* %i, align 4
  %59 = load i32, i32* %8, align 4
  %60 = icmp slt i32 %58, %59
  store i1 %60, i1* %9, align 1
  %61 = load i1, i1* %9, align 1
  br i1 %61, label %for, label %for.end

for:                                              ; preds = %for.cond
  store i32 4, i32* %10, align 4
  %62 = load i32, i32* %10, align 4
  %63 = load i32, i32* %q, align 4
  %64 = mul i32 %62, %63
  store i32 %64, i32* %11, align 4
  %65 = load i32, i32* %11, align 4
  %66 = load i32, i32* %r, align 4
  %67 = add i32 %65, %66
  %68 = load i32, i32* %t, align 4
  %69 = sub i32 %67, %68
  store i32 %69, i32* %12, align 4
  %70 = load i32, i32* %m, align 4
  %71 = load i32, i32* %t, align 4
  %72 = mul i32 %70, %71
  store i32 %72, i32* %13, align 4
  %73 = load i32, i32* %12, align 4
  %74 = load i32, i32* %13, align 4
  %75 = icmp slt i32 %73, %74
  store i1 %75, i1* %14, align 1
  %76 = load i1, i1* %14, align 1
  br i1 %76, label %if.then, label %if.else

if.then:                                          ; preds = %for
  %77 = load i32, i32* %outputCounter, align 4
  %78 = add i32 %77, 1
  store i32 %78, i32* %outputCounter, align 4
  store i32 10, i32* %15, align 4
  %79 = load i32, i32* %15, align 4
  %80 = load i32, i32* %q, align 4
  %81 = mul i32 %79, %80
  store i32 %81, i32* %16, align 4
  %82 = load i32, i32* %16, align 4
  store i32 %82, i32* %q, align 4
  store i32 10, i32* %17, align 4
  %83 = load i32, i32* %17, align 4
  %84 = load i32, i32* %r, align 4
  %85 = load i32, i32* %m, align 4
  %86 = load i32, i32* %t, align 4
  %87 = mul i32 %85, %86
  store i32 %87, i32* %18, align 4
  %88 = load i32, i32* %18, align 4
  %89 = sub i32 %84, %88
  store i32 %89, i32* %19, align 4
  %90 = load i32, i32* %19, align 4
  %91 = mul i32 %83, %90
  store i32 %91, i32* %20, align 4
  %92 = load i32, i32* %20, align 4
  store i32 %92, i32* %r, align 4
  %93 = load i32, i32* %t, align 4
  store i32 %93, i32* %t, align 4
  %94 = load i32, i32* %k, align 4
  store i32 %94, i32* %k, align 4
  store i32 10, i32* %21, align 4
  %95 = load i32, i32* %21, align 4
  store i32 3, i32* %22, align 4
  %96 = load i32, i32* %22, align 4
  %97 = load i32, i32* %q, align 4
  %98 = mul i32 %96, %97
  store i32 %98, i32* %23, align 4
  %99 = load i32, i32* %23, align 4
  %100 = load i32, i32* %r, align 4
  %101 = add i32 %99, %100
  store i32 %101, i32* %24, align 4
  %102 = load i32, i32* %24, align 4
  %103 = mul i32 %95, %102
  store i32 %103, i32* %25, align 4
  %104 = load i32, i32* %25, align 4
  %105 = load i32, i32* %t, align 4
  %106 = sdiv i32 %104, %105
  store i32 %106, i32* %26, align 4
  %107 = load i32, i32* %26, align 4
  store i32 10, i32* %27, align 4
  %108 = load i32, i32* %27, align 4
  %109 = load i32, i32* %m, align 4
  %110 = mul i32 %108, %109
  store i32 %110, i32* %28, align 4
  %111 = load i32, i32* %28, align 4
  %112 = sub i32 %107, %111
  store i32 %112, i32* %29, align 4
  %113 = load i32, i32* %29, align 4
  store i32 %113, i32* %m, align 4
  %114 = load i32, i32* %x, align 4
  store i32 %114, i32* %x, align 4
  br label %if.end

if.else:                                          ; preds = %for
  %115 = load i32, i32* %q, align 4
  %116 = load i32, i32* %k, align 4
  %117 = mul i32 %115, %116
  store i32 %117, i32* %30, align 4
  %118 = load i32, i32* %30, align 4
  store i32 %118, i32* %q, align 4
  store i32 2, i32* %31, align 4
  %119 = load i32, i32* %31, align 4
  %120 = load i32, i32* %q, align 4
  %121 = mul i32 %119, %120
  store i32 %121, i32* %32, align 4
  %122 = load i32, i32* %32, align 4
  %123 = load i32, i32* %r, align 4
  %124 = add i32 %122, %123
  store i32 %124, i32* %33, align 4
  %125 = load i32, i32* %33, align 4
  %126 = load i32, i32* %x, align 4
  %127 = mul i32 %125, %126
  store i32 %127, i32* %34, align 4
  %128 = load i32, i32* %34, align 4
  store i32 %128, i32* %r, align 4
  %129 = load i32, i32* %t, align 4
  %130 = load i32, i32* %x, align 4
  %131 = mul i32 %129, %130
  store i32 %131, i32* %35, align 4
  %132 = load i32, i32* %35, align 4
  store i32 %132, i32* %t, align 4
  %133 = load i32, i32* %k, align 4
  store i32 1, i32* %36, align 4
  %134 = load i32, i32* %36, align 4
  %135 = add i32 %133, %134
  store i32 %135, i32* %37, align 4
  %136 = load i32, i32* %37, align 4
  store i32 %136, i32* %k, align 4
  %137 = load i32, i32* %q, align 4
  store i32 7, i32* %38, align 4
  %138 = load i32, i32* %38, align 4
  %139 = load i32, i32* %k, align 4
  %140 = mul i32 %138, %139
  store i32 %140, i32* %39, align 4
  %141 = load i32, i32* %39, align 4
  store i32 2, i32* %40, align 4
  %142 = load i32, i32* %40, align 4
  %143 = add i32 %141, %142
  store i32 %143, i32* %41, align 4
  %144 = load i32, i32* %41, align 4
  %145 = mul i32 %137, %144
  store i32 %145, i32* %42, align 4
  %146 = load i32, i32* %42, align 4
  %147 = load i32, i32* %r, align 4
  %148 = load i32, i32* %x, align 4
  %149 = mul i32 %147, %148
  store i32 %149, i32* %43, align 4
  %150 = load i32, i32* %43, align 4
  %151 = add i32 %146, %150
  store i32 %151, i32* %44, align 4
  %152 = load i32, i32* %44, align 4
  %153 = load i32, i32* %t, align 4
  %154 = load i32, i32* %x, align 4
  %155 = mul i32 %153, %154
  store i32 %155, i32* %45, align 4
  %156 = load i32, i32* %45, align 4
  %157 = sdiv i32 %152, %156
  store i32 %157, i32* %46, align 4
  %158 = load i32, i32* %46, align 4
  store i32 %158, i32* %m, align 4
  %159 = load i32, i32* %x, align 4
  store i32 2, i32* %47, align 4
  %160 = load i32, i32* %47, align 4
  %161 = add i32 %159, %160
  store i32 %161, i32* %48, align 4
  %162 = load i32, i32* %48, align 4
  store i32 %162, i32* %x, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %163 = load i32, i32* %i, align 4
  %164 = add i32 %163, 1
  store i32 %164, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 3, i32* %49, align 4
  %165 = load i32, i32* %49, align 4
  ret i32 %165
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %1 = call i32 @"makePi()"()
  store i32 %1, i32* %0, align 4
  %2 = load i32, i32* %result, align 4
  ret i32 %2
}

attributes #0 = { nounwind }
