

library(data.table)

################################
##### function definitions #####
################################


create_table <- function(strings, separator)
{
	separated <- unlist(strsplit(strings[1], split = separator))
	result <- data.frame(matrix(0, nrow = length(strings), ncol = length(separated)))
	
	for(i in 1:length(strings))
	{
		separated <- unlist(strsplit(strings[i], split = separator))
		for(j in 1:length(separated))
		{
			result[[j]][i] <- as.numeric(separated[j])
		}
	}
	
	return(result)
}

# vector_string <- vertex_lines[1]
get_vector <- function(vector_string, separator = " ")
{
	separated <- unlist(strsplit(x = vector_string, split = separator))
	separated <- separated[2:length(separated)]
	
	return(paste(separated, collapse = "\t"))
}


print_vertex_data <- function(file_content)
{
	
	file_content <- file_content[-grep(pattern = "# ", x = file_content)]
	
	vertex_lines <- file_content[grep(pattern = "v ", x = file_content)]
	texcoord_lines <- file_content[grep(pattern = "vt ", x = file_content)]
	normal_lines <- file_content[grep(pattern = "vn ", x = file_content)]
	faces_lines <- file_content[grep(pattern = "f ", x = file_content)]
	
	total_vertex_count <- 4*length(faces_lines)
	total_index_count <- 6*length(faces_lines)
	
	for(face in 1:length(faces_lines))
	{
		current_face <- unlist(strsplit(x = faces_lines[face], split = " "))
		current_face <- current_face[2:length(current_face)]
		
		face_table <- create_table(current_face, "/")
		
		for(i in 1:nrow(face_table))
		{
			cat(get_vector(vertex_lines[face_table[[1]]][i]))
			cat("\t")
			cat(get_vector(normal_lines[face_table[[3]]][i]))
			cat("\t")
			cat(get_vector(texcoord_lines[face_table[[2]]][i]))
			cat("\n")
		}
	}
}

###############################
##### main transformation #####
###############################

# set input and output files
# input_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/export_with_comments.obj"
# output_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/transformed.txt"

input_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/export_with_comments.obj"
output_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/monkey_head.txt"



file_content <- readLines(input_file)

file_content <- file_content[-grep(pattern = "# ", x = file_content)]

vertex_lines <- file_content[grep(pattern = "v ", x = file_content)]
texcoord_lines <- file_content[grep(pattern = "vt ", x = file_content)]
normal_lines <- file_content[grep(pattern = "vn ", x = file_content)]
faces_lines <- file_content[grep(pattern = "f ", x = file_content)]

total_vertex_count <- 4*length(faces_lines)
total_index_count <- 6*length(faces_lines)

sink(output_file)

cat(total_vertex_count, "\t", total_index_count, "\n\n")

# print vertices #
for(face in 1:length(faces_lines))
{
	current_face <- unlist(strsplit(x = faces_lines[face], split = " "))
	current_face <- current_face[2:length(current_face)]
	
	face_table <- create_table(current_face, "/")
	
	for(i in 1:nrow(face_table))
	{
		cat(get_vector(vertex_lines[face_table[[1]]][i]))
		cat("\t")
		cat(get_vector(normal_lines[face_table[[3]]][i]))
		cat("\t")
		cat(get_vector(texcoord_lines[face_table[[2]]][i]))
		cat("\n")
	}
}
cat("\n")

# print indices #
offset <- 0
for(face in 1:length(faces_lines))
{
	i1 <- 0
	i2 <- 1
	i3 <- 2
	i4 <- 3
	
	cat(i1+offset, "\t",i2+offset, "\t",i3+offset, "\n")
	cat(i1+offset, "\t",i3+offset, "\t",i4+offset, "\n")
	
	offset <- offset+4
}

sink()






































