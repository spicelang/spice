; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.D = type { %struct.A, %struct.C, i32 }
%struct.A = type { i32 }
%struct.C = type { %struct.A, %struct.B, i32 }
%struct.B = type { i32 }

@printf.str.0 = private unnamed_addr constant [12 x i8] c"%d, %d, %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %d = alloca %struct.D, align 8
  store i32 0, ptr %result, align 4
  store %struct.D zeroinitializer, ptr %d, align 4
  %f1_addr = getelementptr inbounds %struct.D, ptr %d, i32 0, i32 0, i32 0
  store i32 1, ptr %f1_addr, align 4
  %f2_addr = getelementptr inbounds %struct.D, ptr %d, i32 0, i32 2
  store i32 2, ptr %f2_addr, align 4
  %f3_addr = getelementptr inbounds %struct.D, ptr %d, i32 0, i32 1, i32 1, i32 0
  store i32 3, ptr %f3_addr, align 4
  %f1_addr1 = getelementptr inbounds %struct.D, ptr %d, i32 0, i32 0, i32 0
  %1 = load i32, ptr %f1_addr1, align 4
  %f2_addr2 = getelementptr inbounds %struct.D, ptr %d, i32 0, i32 2
  %2 = load i32, ptr %f2_addr2, align 4
  %f3_addr3 = getelementptr inbounds %struct.D, ptr %d, i32 0, i32 1, i32 1, i32 0
  %3 = load i32, ptr %f3_addr3, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1, i32 %2, i32 %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
