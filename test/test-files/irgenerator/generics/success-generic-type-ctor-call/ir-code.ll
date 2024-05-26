; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Nested = type { i32 }
%struct.Test = type {}

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define private void @_ZN6Nested4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.Nested, ptr %2, i32 0, i32 0
  store i32 1, ptr %3, align 4
  ret void
}

define private void @_ZN4TestI6NestedE8testFuncEv(ptr noundef nonnull align 1 %0) {
  %this = alloca ptr, align 8
  %t = alloca %struct.Nested, align 8
  store ptr %0, ptr %this, align 8
  call void @_ZN6Nested4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %t)
  %i_addr = getelementptr inbounds %struct.Nested, ptr %t, i32 0, i32 0
  %2 = load i32, ptr %i_addr, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %test = alloca %struct.Test, align 8
  store i32 0, ptr %result, align 4
  store %struct.Test zeroinitializer, ptr %test, align 1
  call void @_ZN4TestI6NestedE8testFuncEv(ptr noundef nonnull align 1 %test)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
