; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Vector = type { ptr, i32 }

@printf.str.0 = private unnamed_addr constant [24 x i8] c"Capacity of vector: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %dbl = alloca double, align 8
  %doubleVec = alloca %struct.Vector, align 8
  store i32 0, ptr %result, align 4
  store double 3.467000e+00, ptr %dbl, align 8
  store ptr %dbl, ptr %doubleVec, align 8
  %1 = getelementptr inbounds %struct.Vector, ptr %doubleVec, i32 0, i32 1
  store i32 1, ptr %1, align 4
  %cap_addr = getelementptr inbounds %struct.Vector, ptr %doubleVec, i64 0, i32 1
  %2 = load i32, ptr %cap_addr, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
