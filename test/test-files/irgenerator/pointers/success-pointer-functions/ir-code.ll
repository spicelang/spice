; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Person = type { ptr, ptr, i32 }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Person: %s, %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [25 x i8] c"Age before birthday: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [24 x i8] c"Age after birthday: %d\0A\00", align 1

define private void @_Z8birthdayP6Person(ptr %0) {
  %person = alloca ptr, align 8
  store ptr %0, ptr %person, align 8
  %2 = load ptr, ptr %person, align 8
  %age_addr = getelementptr inbounds %struct.Person, ptr %2, i32 0, i32 2
  %3 = load i32, ptr %age_addr, align 4
  %4 = add i32 %3, 1
  store i32 %4, ptr %age_addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %mike = alloca %struct.Person, align 8
  store i32 0, ptr %result, align 4
  store ptr @anon.string.0, ptr %mike, align 8
  %1 = getelementptr inbounds %struct.Person, ptr %mike, i32 0, i32 1
  store ptr @anon.string.1, ptr %1, align 8
  %2 = getelementptr inbounds %struct.Person, ptr %mike, i32 0, i32 2
  store i32 32, ptr %2, align 4
  %lastName_addr = getelementptr inbounds %struct.Person, ptr %mike, i32 0, i32 1
  %3 = load ptr, ptr %lastName_addr, align 8
  %firstName_addr = getelementptr inbounds %struct.Person, ptr %mike, i32 0, i32 0
  %4 = load ptr, ptr %firstName_addr, align 8
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %3, ptr %4)
  %age_addr = getelementptr inbounds %struct.Person, ptr %mike, i32 0, i32 2
  %6 = load i32, ptr %age_addr, align 4
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %6)
  call void @_Z8birthdayP6Person(ptr %mike)
  %age_addr1 = getelementptr inbounds %struct.Person, ptr %mike, i32 0, i32 2
  %8 = load i32, ptr %age_addr1, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
